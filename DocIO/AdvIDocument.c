/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include "advsys-config.h"

#ifdef HAVE_ORBIT

#include <Adv/AdvTypes.h>
#include <Adv/AdvUtil.h>
#include <Adv/adv-idl.h>
#include "AdvDocument.h"
#include "AdvDocumentPrivate.h"
#include "AdvIDocument.h"

/*======================================================================
  CORBA I/F Implementation Function ProtoTypes
  ======================================================================*/

CORBA_char*
DocAccessor_GetDocumentID (PortableServer_Servant _servant,
			   CORBA_Environment * ev);

CORBA_boolean
DocAccessor_GetNthProperty (PortableServer_Servant _servant,
			    CORBA_long n, CORBA_char ** key,
			    CORBA_char ** val,
			    CORBA_Environment * ev);

CORBA_boolean
DocAccessor_GetProperty (PortableServer_Servant _servant,
			 const CORBA_char * key, CORBA_char ** val,
			 CORBA_Environment * ev);

void
DocAccessor_SetProperty (PortableServer_Servant _servant,
			 const CORBA_char * key,
			 const CORBA_char * val,
			 CORBA_Environment * ev);

void
DocAccessor_UnsetNthProperty (PortableServer_Servant _servant, CORBA_long n,
			      CORBA_Environment * ev);

CORBA_long
DocAccessor_GetSize (PortableServer_Servant _servant,
		     CORBA_Environment * ev);

CORBA_long
DocAccessor_GetOctet (PortableServer_Servant _servant,
		      Adv_Int64 offset, Adv_Int32 length,
		      Adv_OctetSeq ** buffer, CORBA_Environment * ev);

CORBA_long
DocAccessor_SetOctet (PortableServer_Servant _servant,
		      Adv_Int64 offset, const Adv_OctetSeq * buffer,
		      CORBA_Environment * ev);

CORBA_long
DocStorage_GetNumDocuments (PortableServer_Servant _servant,
			    CORBA_Environment * ev);

void
DocStorage_Clear (PortableServer_Servant _servant, CORBA_Environment * ev);

AdvIDocument*
DocStorage_Create_local (PortableServer_Servant _servant,
			 const CORBA_char * documentid,
			 CORBA_Environment * ev);

Adv_DocAccessor
DocStorage_Create (PortableServer_Servant _servant,
		   const CORBA_char * documentid,
		   CORBA_Environment * ev);

Adv_DocAccessor
DocStorage_OpenNth (PortableServer_Servant _servant,
		    CORBA_long n, CORBA_Environment * ev);

Adv_DocAccessor
DocStorage_OpenByDocumentID (PortableServer_Servant _servant,
			     const CORBA_char * documentid,
			     CORBA_Environment * ev);

Adv_DocAccessor
DocStorage_OpenByProperty (PortableServer_Servant _servant,
			   Adv_DocAccessor prev,
			   const Adv_StringSeq * properties,
			   CORBA_Environment * ev);

AdvIDocument*
DocStorage_OpenByProperty_local (PortableServer_Servant _servant,
				 Adv_DocAccessor prev,
				 const Adv_StringSeq * properties);

/*======================================================================
  Static Function ProtoTypes
  ======================================================================*/

static void          adv_idio_file_resist(AdvIDocFile* dfile);
static AdvIDocFile*  adv_idio_file_lookup(POA_Adv_DocStorage* svt);
static void          adv_idio_doc_resist(AdvIDocument* doc);
static AdvIDocument* adv_idio_doc_lookup(POA_Adv_DocAccessor* svt);

static AdvIDocument* adv_idio_new(const char* did);
static AdvIDocument* adv_idio_construct(Adv_DocAccessor obj);

static void          adv_idio_page_sync (AdvIDocument* doc);
static void          adv_idio_page_map  (AdvIDocument* doc, int32 pagenum);

/*======================================================================
  File Scope Variables
  ======================================================================*/

static PortableServer_ServantBase__epv DocAccessor_base_epv =
{ NULL, NULL, NULL };

static POA_Adv_DocAccessor__epv DocAccessor_epv =
{
  NULL,
  DocAccessor_GetDocumentID,
  DocAccessor_GetNthProperty,
  DocAccessor_GetProperty,
  DocAccessor_SetProperty,
  DocAccessor_UnsetNthProperty,
  DocAccessor_GetSize,
  DocAccessor_GetOctet,
  DocAccessor_SetOctet,
};

static POA_Adv_DocAccessor__vepv DocAccessor_vepv =
{
  &DocAccessor_base_epv,
  &DocAccessor_epv,
};


static PortableServer_ServantBase__epv DocStorage_base_epv =
{ NULL, NULL, NULL };

static POA_Adv_DocStorage__epv DocStorage_epv =
{
  NULL,
  DocStorage_GetNumDocuments,
  DocStorage_Clear,
  DocStorage_Create,
  DocStorage_OpenNth,
  DocStorage_OpenByDocumentID,
  DocStorage_OpenByProperty,
};

static POA_Adv_DocStorage__vepv DocStorage_vepv = 
{
  &DocStorage_base_epv,
  &DocStorage_epv,
};

static bool module_initialized = false;
static CORBA_ORB orb = NULL;
static CORBA_Environment env;
static PortableServer_POA poa;

static AList* docfile_list = NULL;
static AList* document_list = NULL;

static int pagesize  = 128*1024;
static int blocksize = 1024*1024*4;

struct _AdvIDocument
{
  bool keep_alive;		  /* do not destroy on adv_idio_close(), cuz owned by DocFile */
  POA_Adv_DocAccessor* svt;
  PortableServer_ObjectId* oid;
  Adv_DocAccessor obj;
  char* ior;

  char* did;
  AList* registry;
  int32 pagenum;
  int32 pagefill;
  byte* pagebuf;
  bool pagedirty;

  AList* blocklist;
  int32 datasize;
};

struct _AdvIDocFile
{
  POA_Adv_DocStorage* svt;
  PortableServer_ObjectId* oid;
  Adv_DocStorage obj;
  char* ior;

  AList* doclist;
};

typedef struct _Property
{
  char* key;
  char* val;
} Property;

/*======================================================================
  CORBA I/F Implementation Functions
  ======================================================================*/

CORBA_char*
DocAccessor_GetDocumentID (PortableServer_Servant _servant,
			   CORBA_Environment * ev)
{
  AdvIDocument* doc;
  doc = adv_idio_doc_lookup((POA_Adv_DocAccessor*)_servant);
  return CORBA_string_dup(doc->did);
}

CORBA_boolean
DocAccessor_GetNthProperty (PortableServer_Servant _servant,
			    CORBA_long n,
			    CORBA_char ** key,
			    CORBA_char ** val,
			    CORBA_Environment * ev)
{
  AdvIDocument* doc;
  CORBA_boolean retval;
  int len;

  doc = adv_idio_doc_lookup((POA_Adv_DocAccessor*)_servant);
  len = a_list_length(doc->registry);
  
  if (n < len)
    {
      AList* l;
      int i;
      Property* p;
      l = doc->registry;
      for (i = 0; i < n; i++)
        l = l->next;
      p = (Property*)l->data;
      *key = CORBA_string_dup(p->key);
      *val = CORBA_string_dup(p->val);
      retval = true;
    }
  else
    {
      *key = CORBA_string_dup("");
      *val = CORBA_string_dup("");
      retval = false;
    }

  return retval; 
}

CORBA_boolean
DocAccessor_GetProperty (PortableServer_Servant _servant,
			 const CORBA_char * key, CORBA_char ** val,
			 CORBA_Environment * ev)
{
  AdvIDocument* doc;
  AList* l;

  doc = adv_idio_doc_lookup((POA_Adv_DocAccessor*)_servant);
  for (l = doc->registry; l != NULL; l = l->next)
    {
      Property* p;
      p = (Property*)l->data;
      if (strcmp(p->key, key) == 0)
        {
          *val = CORBA_string_dup(p->val);
          return true;
        }
    }
  *val = NULL;
  return false;
}

void
DocAccessor_SetProperty (PortableServer_Servant _servant,
			 const CORBA_char * key,
			 const CORBA_char * val, CORBA_Environment * ev)
{
  AList* l;
  Property* p;
  AdvIDocument* doc;
  doc = adv_idio_doc_lookup((POA_Adv_DocAccessor*)_servant);

  for (l = doc->registry; l != NULL; l = l->next)
    {
      p = (Property*)l->data;
      if (strcmp(p->key, key) == 0)
        {
          free(p->val);
          p->val = strdup(val);
          return;
        }
    }
  p = a_new(Property);
  p->key = strdup(key);
  p->val = strdup(val);
  doc->registry = a_list_prepend(doc->registry, p);
}

void
DocAccessor_UnsetNthProperty (PortableServer_Servant _servant, CORBA_long n,
			      CORBA_Environment * ev)
{
  AdvIDocument* doc;
  int len;

  doc = adv_idio_doc_lookup((POA_Adv_DocAccessor*)_servant);
  len = a_list_length(doc->registry);
  
  if (n < len)
    {
      AList* l;
      int i;
      Property* p;
      for (i = 0; i < n; i++)
        l = l->next;
      p = (Property*)l->data;
      free(p->key);
      free(p->val);
      a_free(p);
      doc->registry = a_list_remove(l);
    }
}

CORBA_long
DocAccessor_GetSize (PortableServer_Servant _servant,
		     CORBA_Environment * ev)
{
  AdvIDocument* doc;
  doc = adv_idio_doc_lookup((POA_Adv_DocAccessor*)_servant);
  //printf("Datasize %d\n", (long)doc->datasize);
  return doc->datasize;
}

CORBA_long
DocStorage_GetNumDocuments (PortableServer_Servant _servant,
			    CORBA_Environment * ev)
{
  AdvIDocFile* dfile;
  dfile = adv_idio_file_lookup((POA_Adv_DocStorage*)_servant);
  return a_list_length(dfile->doclist);
}

void
DocStorage_Clear (PortableServer_Servant _servant, CORBA_Environment * ev)
{
  AList* i;
  AdvIDocFile* dfile;
  dfile = adv_idio_file_lookup((POA_Adv_DocStorage*)_servant);
  for (i = dfile->doclist; i != NULL; i = i->next)
    {
      AdvIDocument* doc;
      doc = (AdvIDocument*)i->data;
      doc->keep_alive = false;
      adv_idio_close(doc);
    }
  a_list_destroy(dfile->doclist);
  dfile->doclist = NULL;
}

AdvIDocument*
DocStorage_Create_local (PortableServer_Servant _servant,
			 const CORBA_char * documentid,
			 CORBA_Environment * ev)
{
  AdvIDocument* doc;
  AdvIDocFile* dfile;
  dfile = adv_idio_file_lookup((POA_Adv_DocStorage*)_servant);
  doc = adv_idio_new(documentid);
  dfile->doclist = a_list_prepend(dfile->doclist, doc);
  //printf("Append %p, %p\n", doc, doc->svt);
  return doc;
}

Adv_DocAccessor
DocStorage_Create (PortableServer_Servant _servant,
		   const CORBA_char * documentid,
		   CORBA_Environment * ev)
{
  AdvIDocument* doc;
  doc = DocStorage_Create_local(_servant, documentid, ev);
  return CORBA_Object_duplicate(doc->obj, &env);
}

Adv_DocAccessor
DocStorage_OpenNth (PortableServer_Servant _servant,
		    CORBA_long n, CORBA_Environment * ev)
{
  AList* l;
  AdvIDocFile* dfile;
  dfile = adv_idio_file_lookup((POA_Adv_DocStorage*)_servant);
  l = a_list_nth(dfile->doclist, n);
  if (l)
    {
      AdvIDocument* doc;
      Adv_DocAccessor obj;
      doc = (AdvIDocument*)(l->data);
      obj = PortableServer_POA_servant_to_reference(poa, doc->svt, &env);
      return CORBA_Object_duplicate(obj, &env);
    }
  else
    return NULL;
}

Adv_DocAccessor
DocStorage_OpenByDocumentID (PortableServer_Servant _servant,
			     const CORBA_char * documentid,
			     CORBA_Environment * ev)
{
  AList* l;
  AdvIDocFile* dfile;
  dfile = adv_idio_file_lookup((POA_Adv_DocStorage*)_servant);
  for (l = dfile->doclist; l != NULL; l = l->next)
    {
      AdvIDocument* doc = (AdvIDocument*)l->data;
      if (strcmp(documentid, doc->did) == 0)
	break;
    }
  if (l)
    {
      AdvIDocument* doc;
      Adv_DocAccessor obj;
      doc = (AdvIDocument*)(l->data);
      obj = PortableServer_POA_servant_to_reference(poa, doc->svt, &env);
      return CORBA_Object_duplicate(obj, &env);
    }
  else
    return NULL;
}

AdvIDocument*
DocStorage_OpenByProperty_local (PortableServer_Servant _servant,
				 Adv_DocAccessor prev,
				 const Adv_StringSeq * properties)
{
  /* not implemented */
  AdvIDocFile* dfile;
  AdvIDocument* prevdoc;
  AList* l;
  bool b;
  const char* key;
  const char* val;
  const char* v;
  AdvIDocument* ret;
  dfile = adv_idio_file_lookup((POA_Adv_DocStorage*)_servant);

  ret = NULL;
  b = !prev ? true : false;
  for (l = dfile->doclist; l != NULL; l = l->next)
    {
      int n;
      AdvIDocument* doc;
      doc = (AdvIDocument*)l->data;
      n = 0;
      while (true)
	{
	  key = val = NULL;
	  if (n >= properties->_length)
	    break;
	  key = properties->_buffer[n];
	  val = properties->_buffer[n+1];
	  v = adv_idio_get_property(doc, key);
	  if (v && strcmp(v, val) != 0)
	    break;
	  n += 2;
	}
      if (n >= properties->_length)
	{
	  if (b)
	    {
	      ret = doc;
	      break;
	    }
	  else if (CORBA_Object_is_equivalent(prev, doc->obj, &env))
	    b = true;
	}
    }
  /* !!! NEEDS FREE properties ? !!! */
  CORBA_free((Adv_StringSeq *)properties);

  return ret;
}

Adv_DocAccessor
DocStorage_OpenByProperty (PortableServer_Servant _servant,
			   Adv_DocAccessor prev,
			   const Adv_StringSeq * properties,
			   CORBA_Environment * ev)
{
  AdvIDocument* doc;
  doc = DocStorage_OpenByProperty_local(_servant, prev, properties);
  if (doc)
    return CORBA_Object_duplicate(doc->obj, &env);
  else
    return NULL;
}


/*------------------------------------------------------------*/
byte*
DocAccessor_GetNthBlock_local (AdvIDocument* doc, int n)
{
  int i;
  int current_length;
  AList* l;

  current_length = a_list_length(doc->blocklist);
  if (!(n < current_length))
    {
      int m = n - current_length + 1;
      for (i = 0; i < m; i++)
	doc->blocklist = a_list_prepend(doc->blocklist, NULL);
    }

  l = doc->blocklist;
  for (i = 0; i < n; i++)
    l = l->next;
  if (l->data == NULL)
    {
      byte* newblock = a_new_array(byte, blocksize);
      l->data = newblock;
    }
  return l->data;
}

CORBA_long
DocAccessor_GetOctet_local (AdvIDocument* doc,
			    int64 offset, int32 length,
			    octet* buffer)
{
  int o;
  CORBA_long left;
  //printf("GetOctet_local offset=%08X, length=%d\n", (int32)offset, length);
  if (offset + length > doc->datasize)
    length = doc->datasize - offset;
  if (length < 0)
    length = 0;
  left = length;
  o = 0;
  while (left > 0)
    {
      int blockindex         = (offset+o) / blocksize;
      CORBA_long blockoffset = (offset+o) % blocksize;
      CORBA_long blockleft   = blocksize - blockoffset;
      CORBA_long to_read     = left < blockleft ? left : blockleft;
      byte* block = DocAccessor_GetNthBlock_local(doc, blockindex);
      memcpy(buffer+o, block+blockoffset, to_read);
      o    += to_read;
      left -= to_read;
      //printf("GetOctet_local : page=%d, offset=%08X, length=%d\n", blockindex, blockoffset, blockleft);
    }
  return length; 
}

CORBA_long
DocAccessor_GetOctet (PortableServer_Servant _servant,
		      Adv_Int64 offset, Adv_Int32 length,
		      Adv_OctetSeq ** buffer, CORBA_Environment * ev)
{
  AdvIDocument* doc;
  CORBA_long len;
  //printf("GetOctet offset=%08X, lengthh=%d, %d\n", (int32)offset, length, ev->_major);
  doc = adv_idio_doc_lookup(_servant);


  *buffer = Adv_OctetSeq__alloc();
  (*buffer)->_length = (*buffer)->_maximum = length;
  (*buffer)->_buffer = CORBA_sequence_CORBA_octet_allocbuf(length);
  CORBA_sequence_set_release(*buffer, true);
  len = DocAccessor_GetOctet_local(doc, offset, length, (*buffer)->_buffer);
  (*buffer)->_length = len;
  return len;
}

/*------------------------------------------------------------*/

CORBA_long
DocAccessor_SetOctet_local (AdvIDocument* doc,
			    int32 offset,
			    int32 length,
			    const octet* buffer)
{
  int32 left;

  left = length;
  
  if (offset + length > doc->datasize)
    doc->datasize = offset + length;
  while (left > 0)
    {
      int blockindex = offset / blocksize;
      int32 blockoffset = offset % blocksize;
      int32 blockleft   = blocksize - blockoffset;
      int to_write = left < blockleft ? left : blockleft;
      byte* block = DocAccessor_GetNthBlock_local(doc, blockindex);
      memcpy(block+blockoffset, buffer, to_write);
      buffer += to_write;
      offset += to_write;
      left -= to_write;
    }
  return length;
}

CORBA_long
DocAccessor_SetOctet (PortableServer_Servant _servant,
		      Adv_Int64 offset,
		      const Adv_OctetSeq * buffer,
		      CORBA_Environment * ev)
{
  AdvIDocument* doc;
  doc = adv_idio_doc_lookup(_servant);
  return DocAccessor_SetOctet_local (doc, offset, buffer->_length, buffer->_buffer);
}

/*======================================================================
  Functions
  ======================================================================*/

/*------------------------------------------------------------
  static functions
  ------------------------------------------------------------*/

void
adv_idio_file_resist(AdvIDocFile* dfile)
{
  docfile_list = a_list_prepend(docfile_list, dfile);
}

AdvIDocFile*
adv_idio_file_lookup(POA_Adv_DocStorage* svt)
{
  AList* i;
  for (i = docfile_list; i != NULL; i = i->next)
    {
      AdvIDocFile* dfile;
      dfile = (AdvIDocFile*)(i->data);
      if (dfile->svt == svt)
	return dfile;
    }
  return NULL;
}

void
adv_idio_doc_resist(AdvIDocument* doc)
{
  document_list = a_list_prepend(document_list, doc);
}

AdvIDocument*
adv_idio_doc_lookup(POA_Adv_DocAccessor* svt)
{
  AList* i;
  for (i = document_list; i != NULL; i = i->next)
    {
      AdvIDocument* doc;
      doc = (AdvIDocument*)(i->data);
      if (doc->svt == svt)
	return doc;
    }
  return NULL;
}

/*------------------------------------------------------------
  public functions
  ------------------------------------------------------------*/

void
adv_idio_init(int* argc, char*** argv)
{
  if (!module_initialized)
    {
      module_initialized = true;
      CORBA_exception_init(&env);
      orb = CORBA_ORB_init(argc, *argv, "orbit-local-orb", &env);
      poa = (PortableServer_POA)
	CORBA_ORB_resolve_initial_references(orb, "RootPOA", &env);
      PortableServer_POAManager_activate
	(PortableServer_POA__get_the_POAManager(poa, &env), &env);
    }
}

void
adv_idio_run()
{
  CORBA_ORB_run(orb, &env);
}

void
adv_idio_perform_work()
{
  CORBA_ORB_perform_work(orb, &env);
}

/*------------------------------------------------------------*/

AdvIDocFile*
adv_idio_file_open(const char* loc, const char* mode)
{
  AdvIDocFile* dfile;
  if (loc)
    {			  /* remote documentfile */
      Adv_DocStorage obj;
  
      if (strncasecmp(loc, "advdoc:", 7) == 0)
	loc += 7;				  /* skip heading `advdoc:' */
      if (strncasecmp(loc, "IOR:", 4) != 0)
	return NULL;

      obj = CORBA_ORB_string_to_object(orb, (char*)loc, &env);
      if (CORBA_Object_is_nil(obj, &env))
	return NULL;
      if (strstr(obj->object_id, "DocStorage") == 0)
	{
	  CORBA_Object_release(obj, &env);
	  return NULL;
	}
  
      dfile = a_new(AdvIDocFile);
      dfile->svt = NULL;
      dfile->obj = obj;
      dfile->ior = CORBA_ORB_object_to_string(orb, dfile->obj, &env);
      dfile->doclist = NULL;
    }
  else
    {			  /* local in-process documentfile */
      dfile = a_new(AdvIDocFile);
      dfile->svt = a_new(POA_Adv_DocStorage);
      dfile->svt->_private = NULL;
      dfile->svt->vepv = &DocStorage_vepv;
      POA_Adv_DocStorage__init(dfile->svt, &env);
      dfile->oid = PortableServer_POA_activate_object(poa, dfile->svt, &env);
      dfile->obj = PortableServer_POA_servant_to_reference(poa,
							   dfile->svt,
							   &env);
      dfile->ior = CORBA_ORB_object_to_string(orb, dfile->obj, &env);
      dfile->doclist = NULL;
      adv_idio_file_resist(dfile);
    }
  return dfile;
}

void
adv_idio_file_close(AdvIDocFile* dfile)
{
  CORBA_Object_release(dfile->obj, &env);
  CORBA_free(dfile->ior);
  if (dfile->svt)
    {
      AList* l;
      for (l = docfile_list; l != NULL; l = l->next)
        {
          if (l->data == dfile)
     	    {
	      docfile_list = a_list_remove(l);
	      break;
	    }
        }
      PortableServer_POA_deactivate_object(poa, dfile->oid, &env);
      a_free(dfile->svt);
      CORBA_free(dfile->oid);
      for (l = dfile->doclist; l != NULL; l = l->next)
	{
	  AdvIDocument* doc = (AdvIDocument*)l->data;
	  doc->keep_alive = false;
	  adv_idio_close(doc);
	}
      a_list_destroy(dfile->doclist);
    }
  a_free(dfile);
}

void
adv_idio_file_clear(AdvIDocFile* dfile)
{
  if (dfile->svt)
    DocStorage_Clear(dfile->svt, &env);
  else
    Adv_DocStorage_Clear(dfile->obj, &env);
}

/*------------------------------------------------------------*/

const char*
adv_idio_file_get_locator (AdvIDocFile* dfile)
{
  return dfile->ior;
}

/*------------------------------------------------------------*/

AdvIDocument*
adv_idio_create(AdvIDocFile* dfile, const char* did)
{
  AdvIDocument* doc;
  doc = NULL;
  if (dfile->svt)
    doc = DocStorage_Create_local (dfile->svt, (char*)did, &env);
  else
    {
      Adv_DocAccessor obj;
      obj = Adv_DocStorage_Create(dfile->obj, (char*)did, &env);
      doc = adv_idio_construct(obj);
    }
  return doc;
}

AdvIDocument*
adv_idio_new(const char* did)
{
  /* local in-process document */
  AdvIDocument* doc;
  doc = a_new(AdvIDocument);
  doc->keep_alive = true;
  doc->svt = a_new(POA_Adv_DocAccessor);
  doc->svt->_private = NULL;
  doc->svt->vepv = &DocAccessor_vepv;
  POA_Adv_DocAccessor__init(doc->svt, &env);
  doc->oid = PortableServer_POA_activate_object(poa, doc->svt, &env);
  doc->obj = PortableServer_POA_servant_to_reference(poa,
						     doc->svt,
						     &env);
  doc->ior = CORBA_ORB_object_to_string(orb, doc->obj, &env);
  doc->did = CORBA_string_dup((char*)did);
  adv_idio_doc_resist(doc);

  doc->blocklist = NULL;
  doc->datasize = 0;
  doc->registry = a_list_new();

  //printf("Created %s, %p\n", did, doc);

  return doc;
}

AdvIDocument*
adv_idio_construct(Adv_DocAccessor obj)
{
  /* remote document */
  AdvIDocument* doc;
  int i;
  CORBA_char* key;
  CORBA_char* val;

  if (CORBA_Object_is_nil(obj, &env))
    return NULL;
  {
    if (strstr(obj->object_id, "DocAccessor") == NULL)
      {
        CORBA_Object_release(obj, &env);
        return NULL;
      }
  }

  doc = a_new(AdvIDocument);
  doc->keep_alive = false;
  doc->svt = NULL;
  doc->oid = NULL;
  doc->obj = obj;
  doc->ior = CORBA_ORB_object_to_string(orb, doc->obj, &env);
  doc->did = Adv_DocAccessor_GetDocumentID(doc->obj, &env);
  doc->blocklist = NULL;
  doc->datasize = 0;

  doc->pagenum = -1;                             // means unmapped
  doc->pagefill = 0;
  doc->pagebuf = a_new_array(byte, pagesize);
  doc->pagedirty = false;

  doc->registry = a_list_new();

  i = 0;
  while (Adv_DocAccessor_GetNthProperty(doc->obj, i++, &key, &val, &env) == true)
    {
      Property* prop = a_new(Property);
      prop->key = strdup(key);
      prop->val = strdup(val);
      doc->registry = a_list_prepend(doc->registry, prop);
      CORBA_free(key);
      CORBA_free(val);
    }
  CORBA_free(key);
  CORBA_free(val);

  return doc;
}

void
adv_idio_close(AdvIDocument* doc)
{
  AList* l;
  Property* p;
  if (!doc->keep_alive)
    {
      if (doc->svt)
	{
	  AList* l;
	  for (l = document_list; l != NULL; l = l->next)
	    {
	      if (l->data == doc)
		{
		  document_list = a_list_remove(l);
		  break;
		}
            }
	  PortableServer_POA_deactivate_object(poa, doc->oid, &env);
	  CORBA_free(doc->oid);
	  for (l = doc->blocklist; l != NULL; l = l->next)
	    {
	      octet* block = (octet*)(l->data);
	      a_free(block);
	    }
	  a_list_destroy(doc->blocklist);
	  a_free(doc->svt);
	}
      else
	{
	  adv_idio_page_sync(doc);
	  a_free(doc->pagebuf);
	}
      CORBA_free(doc->did);
      CORBA_free(doc->ior);

      for (l = doc->registry; l != NULL; l = l->next)
        {
          p = (Property*)l->data;
          free(p->key);
          free(p->val);
          a_free(p);
        }
      a_list_destroy(doc->registry);
      CORBA_Object_release(doc->obj, &env);
      a_free(doc);
    }
}

AdvIDocument*
adv_idio_open_nth(AdvIDocFile* dfile, int n)
{
  AdvIDocument* doc;
  doc = NULL;
  if (dfile->svt)
    {
      AList* l;
      l = a_list_nth(dfile->doclist, n);
      if (l)
	doc = (AdvIDocument*)l->data;
    }
  else
    {
      Adv_DocAccessor obj;
      obj = Adv_DocStorage_OpenNth(dfile->obj, n, &env);
      doc = adv_idio_construct(obj);
    }
  return doc;
}

AdvIDocument*
adv_idio_open_by_locator(const char* locator)
{
  AdvIDocument* doc;
  Adv_DocAccessor obj;
  doc = NULL;
  obj = CORBA_ORB_string_to_object(orb, (char*)locator, &env);
  if (!CORBA_Object_is_nil(obj, &env))
    doc = adv_idio_construct(obj);
  return doc;
}

AdvIDocument*
adv_idio_open_by_documentid(AdvIDocFile* dfile, const char* did)
{
  AdvIDocument* doc;
  doc = NULL;
  if (dfile->svt)
    {
      AList* l;
      for (l = dfile->doclist; l != NULL; l = l->next)
	{
	  AdvIDocument* doc = (AdvIDocument*)l->data;
	  if (strcmp(did, doc->did) == 0)
	    break;
	}
      if (l)
	doc = (AdvIDocument*)l->data;
    }
  else
    {
      Adv_DocAccessor obj;
      obj = Adv_DocStorage_OpenByDocumentID(dfile->obj, did, &env);
      doc = adv_idio_construct(obj);
    }
  return doc;
}

AdvIDocument*
adv_idio_open_by_property_v(AdvIDocFile* dfile, void* prev, va_list va)
{
  Adv_StringSeq* seq;
  AdvIDocument* doc;
  AdvIDocument* prevdoc;
  int i, n;
  const char* p;
  va_list va2;
  
  prevdoc = (AdvIDocument*)prev;
  va_copy(va2, va);
  n = 0;
  while (p = va_arg(va2, const char*), p)
    n++;
  va_end(va2);
  
  seq = Adv_StringSeq__alloc();
  seq->_length = n;
  seq->_maximum = n;
  seq->_buffer = CORBA_sequence_CORBA_string_allocbuf(n);
  CORBA_sequence_set_release(seq, true);
  va_copy(va2, va);
  for (i = 0; i < n; i++)
    {
      p = va_arg(va2, const char*);
      seq->_buffer[i] = CORBA_string_dup((char*)p);
    }
  va_end(va2);

  if (dfile->svt)
    {
      Adv_DocAccessor prevobj = prevdoc ? prevdoc->obj : NULL;
      doc = DocStorage_OpenByProperty_local(dfile->svt, prevobj, seq);
    }
  else
    {
      Adv_DocAccessor obj;
      obj = Adv_DocStorage_OpenByProperty(dfile->obj, prevdoc->obj, seq, &env);
      doc = adv_idio_construct(obj);
    }
  return doc;
}

/*------------------------------------------------------------*/

const char*
adv_idio_get_documentid (AdvIDocument* doc)
{ return doc->did; }

adv_off_t
adv_idio_get_size (AdvIDocument* doc)
{
  return Adv_DocAccessor_GetSize(doc->obj, &env);
}

const char*
adv_idio_get_locator (AdvIDocument* doc)
{ return doc->ior; }

void
adv_idio_set_property (AdvIDocument* doc,
		       const char* key,
		       const char* val)
{
  AList* l;
  Property* prop;
  for (l = doc->registry; l != NULL; l = l->next)
    {
      prop = (Property*)l->data;
      if (strcmp(prop->key, key) == 0)
	{
	  free(prop->val);
	  prop->val = strdup(val);
	  break;
	}
    }
  if (!l)
    {
      prop = a_new(Property);
      prop->key = strdup(key);
      prop->val = strdup(val);
      doc->registry = a_list_prepend(doc->registry, prop);
    }

  Adv_DocAccessor_SetProperty(doc->obj, (char*)key, (char*)val, &env);
}

void
adv_idio_set_property_int32(AdvIDocument* doc,
			    const char* key,
			    int32 val)
{
  char buf[100];				  /* !! fixed-length-buffer !!*/
  snprintf(buf, sizeof(buf), "%i", val);
  adv_idio_set_property(doc, key, buf);
}

void
adv_idio_set_property_float64(AdvIDocument* doc,
			      const char* key,
			      float64 val)
{
  char buf[200];				  /* !! fixed-length-buffer !!*/
  snprintf(buf, sizeof(buf), "%g", val);
  adv_idio_set_property(doc, key, buf);
}

const char*
adv_idio_get_property (AdvIDocument* doc,
		       const char* key)
{
  AList* l;
  for (l = doc->registry; l != NULL; l = l->next)
    {
      Property* prop = (Property*)l->data;
      if (strcmp(prop->key, key) == 0)
        return prop->val;
    }
  return NULL;
}

void
adv_idio_unset_nth_property (AdvIDocument* doc,
                             int n)
{
  int i;
  AList* l = doc->registry;
  for (i = 0; i < n && l; i++)
    l = l->next;
  if (l)
    {
      Property* p = (Property*)l->data;
      free(p->key);
      free(p->val);
      a_free(p);
      doc->registry = a_list_remove(l);
      Adv_DocAccessor_UnsetNthProperty(doc->obj, n, &env);
    }
}

bool
adv_idio_get_nth_property (AdvIDocument* doc,
                           int n,
                           char* key, int keysize,
                           char* val, int valsize)
{
  int i;
  AList* l;
  Property* p;
  i = 0;
  for (l = doc->registry; l != NULL; l = l->next, i++)
    if (i >= n)
      break;
  if (!l)
    return false;
  p = (Property*)l->data;
  if (key)
    strncpy(key, p->key, keysize);
  if (val)
    strncpy(val, p->val, valsize);
  return true;
}

bool
adv_idio_get_property_int32 (AdvIDocument* doc,
                             const char* key,
                             int32* val)
{
  const char* v = adv_idio_get_property(doc, key);
  if (v)
    {
      *val = atoi(v);
      return true;
    }
  else
    return false;
}

bool
adv_idio_get_property_float64 (AdvIDocument* doc,
                               const char* key,
                               float64* val)
{
  const char* v = adv_idio_get_property(doc, key);
  if (v)
    {
      *val = strtod(v, NULL);
      return true;
    }
  else
    return false;
}
bool
adv_idio_check_property_v(AdvIDocument* doc, va_list va)
{
  const char* key;
  const char* val;
  const char* v;

  while (true)
    {
      key = va_arg(va, const char*);
      val = va_arg(va, const char*);
      if (key == NULL)
        break;
      v = adv_idio_get_property(doc, key);
      if (!v || (v && strcmp(v, val) != 0))
        break;
    }
  if (key == NULL)
    return true;
  else
    return false;
}

bool
adv_idio_check_property(AdvIDocument* doc, ...)
{
  va_list va;
  bool ret;
  va_start(va, doc);
  ret = adv_idio_check_property_v(doc, va);
  va_end(va);
  return ret;
}

/*------------------------------------------------------------*/

static
void
adv_idio_page_sync (AdvIDocument* doc)
{
  Adv_OctetSeq seq;
  if (doc->pagedirty)
    {
      adv_off_t offset = pagesize*doc->pagenum;
      adv_off_t left   = doc->pagefill;
      adv_off_t done   = 0;

      //printf("SetOctet %d\n", left);
      while (left > 0)
        {
          int datagramsize = left < 64*1024 ? left : 64*1024;
          seq._buffer = doc->pagebuf + done;
          seq._maximum = seq._length = datagramsize;
          CORBA_sequence_set_release(&seq, CORBA_FALSE);
          Adv_DocAccessor_SetOctet(doc->obj, offset+done, &seq, &env);

          done += datagramsize;
          left -= datagramsize;
        }

      doc->pagedirty = false;
      //printf("SetOctet done\n");
    }
}

static
void
adv_idio_page_map (AdvIDocument* doc, int32 pagenum)
{
  adv_off_t offset = pagesize*pagenum;
  Adv_OctetSeq* seq;
  if (doc->pagenum != pagenum)
    {
      Adv_DocAccessor_GetOctet(doc->obj, offset, pagesize, &seq, &env);
      //usleep(2);
      if (env._major == CORBA_NO_EXCEPTION)
	{
	  memcpy(doc->pagebuf, seq->_buffer, seq->_length);
	  doc->pagefill = seq->_length;
	  doc->pagenum = pagenum;
	  doc->pagedirty = false;
	  //if (CORBA_sequence_get_release(seq))
	  //CORBA_free(seq->_buffer);
	  CORBA_free(seq);
	}
      else
	{
	  doc->pagefill = 0;
	  doc->pagenum = -1;
	  doc->pagedirty = false;
	  printf("Exception!\n");
	}
    }
}

CORBA_long
adv_idio_read (AdvIDocument* doc, adv_off_t offset, int32 length, void* buf_arg)
{
  CORBA_long ret;
  if (doc->svt)
    ret = DocAccessor_GetOctet_local(doc, offset, length, buf_arg);
  else
    {
      adv_off_t left = length;
      adv_off_t done = 0;
      octet* buf = (octet*)buf_arg;
      bool runout = false;
      while (left > 0 && !runout)
	{
	  int32 page_num    = (offset+done) / pagesize;
	  int32 page_offset = (offset+done) % pagesize;
	  int32 page_left   = pagesize-page_offset;
	  int32 block       = left < page_left ? left : page_left;
	  adv_idio_page_map(doc, page_num);
	  if (page_offset + block > doc->pagefill)
	    {
	      block = doc->pagefill - page_offset;
	      runout = true;
	    }
	  memcpy(buf+done, doc->pagebuf+page_offset, block);
	  left -= block;
	  done += block;
	}
      ret = done;
    }
  return ret;
}

int32
adv_idio_write (AdvIDocument* doc, adv_off_t offset, int32 length, const void* buf_arg)
{
  CORBA_long ret;
  if (doc->svt)
    ret = DocAccessor_SetOctet_local(doc, offset, length, buf_arg);
  else
    {
      adv_off_t left = length;
      adv_off_t done = 0;
      octet* buf = (octet*)buf_arg;
      while (left > 0)
	{
	  int32 page_num    = (offset+done) / pagesize;
	  int32 page_offset = (offset+done) % pagesize;
	  int32 page_left   = pagesize-page_offset;
	  int32 block       = left < page_left ? left : page_left;

	  if (page_num != doc->pagenum)
	    {
	      adv_idio_page_sync(doc);
	      /* if (left - block < pagesize) */
	      adv_idio_page_map(doc, page_num);
	    }

	  if (page_offset + block > doc->pagefill)
	    doc->pagefill = page_offset + block;      /* size extension */

	  memcpy(doc->pagebuf+page_offset, buf+done, block);
	  left -= block;
	  done += block;
	  doc->pagedirty = true;
	}
      ret = done;
    }
  return ret;
}

/*------------------------------------------------------------*/

#define swap_octet(a, b) {octet c; c = a; a = b; b = c;}

int32
adv_idio_read_octet (AdvIDocument* doc,
		     adv_off_t offset,
		     int32  length,
		     octet* buf)
{
  return adv_idio_read(doc, offset, length, buf);
}

int32
adv_idio_read_string_length(AdvIDocument* doc, adv_off_t offset)
{
  int32 length;
  adv_idio_read_int32(doc, offset, &length);
  return length;
}

int32
adv_idio_read_string(AdvIDocument* doc, adv_off_t offset, char* buf)
{
  int32 length;
  int32 a = 0;
  a += adv_idio_read_int32(doc, offset, &length);
  a += adv_idio_read_octet(doc, offset+a, length, buf);
  buf[length] = '\0';
  return a;
}

int32
adv_idio_read_int8 (AdvIDocument* doc, adv_off_t offset, int8* val)
{
  return adv_idio_read(doc, offset, sizeof(int8), val);
}

int32
adv_idio_read_int8v (AdvIDocument* doc, adv_off_t offset, int num, int8* val)
{
  return adv_idio_read(doc, offset, sizeof(int8)*num, val);
}

int32
adv_idio_read_int16 (AdvIDocument* doc, adv_off_t offset, int16* val)
{
  int16 r;
  octet* b;
  r = adv_idio_read(doc, offset, sizeof(int16), val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[1]);
#endif
  return r;
}

int32
adv_idio_read_int16v (AdvIDocument* doc, adv_off_t offset, int num, int16* val)
{
  int i;
  adv_off_t o = 0;
  for (i = 0; i < num; i++)
    o += adv_idio_read_int16(doc, offset+o, val+i);
  return o;
}

int32
adv_idio_read_int32 (AdvIDocument* doc, adv_off_t offset, int32* val)
{
  int32 r;
  octet* b;
  r = adv_idio_read(doc, offset, sizeof(int32), val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return r;
}

int32
adv_idio_read_int32v (AdvIDocument* doc, adv_off_t offset, int num, int32* val)
{
  int i;
  adv_off_t o = 0;
  for (i = 0; i < num; i++)
    o += adv_idio_read_int32(doc, offset+o, val+i);
  return o;
}

int32
adv_idio_read_int64 (AdvIDocument* doc, adv_off_t offset, int64* val)
{
  int64 r;
  octet* b;
  r = adv_idio_read(doc, offset, sizeof(int64), val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[7]);
  swap_octet(b[1], b[6]);
  swap_octet(b[2], b[5]);
  swap_octet(b[3], b[4]);
#endif
  return r;
}

int32
adv_idio_read_int64v (AdvIDocument* doc, adv_off_t offset, int num, int64* val)
{
  int i;
  adv_off_t o = 0;
  for (i = 0; i < num; i++)
    o += adv_idio_read_int64(doc, offset+o, val+i);
  return o;
}

int32
adv_idio_read_float32 (AdvIDocument* doc, adv_off_t offset, float32* val)
{
  float32 r;
  octet* b;
  r = adv_idio_read(doc, offset, sizeof(float32), val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return r;
}

int32
adv_idio_read_float32v (AdvIDocument* doc, adv_off_t offset, int num, float32* val)
{
  int i;
  adv_off_t o = 0;
  for (i = 0; i < num; i++)
    o += adv_idio_read_float32(doc, offset+o, val+i);
  return o;
}

int32
adv_idio_read_float64 (AdvIDocument* doc, adv_off_t offset, float64* val)
{
  float64 r;
  octet* b;
  r = adv_idio_read(doc, offset, sizeof(float64), val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[7]);
  swap_octet(b[1], b[6]);
  swap_octet(b[2], b[5]);
  swap_octet(b[3], b[4]);
#endif
  return r;
}

int32
adv_idio_read_float64v (AdvIDocument* doc, adv_off_t offset, int num, float64* val)
{
  int i;
  adv_off_t o = 0;
  for (i = 0; i < num; i++)
    o += adv_idio_read_float64(doc, offset+o, val+i);
  return o;
}

/*------------------------------------------------------------*/

int32
adv_idio_write_octet (AdvIDocument* doc, adv_off_t offset, int32 length, const octet* buf)
{
  return adv_idio_write(doc, offset, length, buf);
}

int32
adv_idio_write_string(AdvIDocument* doc, adv_off_t offset, const char* buf)
{
  int32 length = strlen(buf);
  adv_idio_write_int32(doc, offset, length);
  return adv_idio_write_octet(doc, offset+sizeof(int32), length, buf) + sizeof(int32);
}

int32
adv_idio_write_int8 (AdvIDocument* doc, adv_off_t offset, int8 val)
{
  octet* b;
  b = (octet*)&val;
  return adv_idio_write(doc, offset, sizeof(int8), b);
}

int32
adv_idio_write_int8v (AdvIDocument* doc, adv_off_t offset, int num, const int8* 
val)
{
  int i;
  adv_off_t o = 0;
  for (i = 0; i < num; i++)
    o += adv_idio_write_int8(doc, offset+o, val[i]);
  return o;
}

int32
adv_idio_write_int16 (AdvIDocument* doc, adv_off_t offset, int16 val)
{
  octet* b;
  b = (octet*)&val;
#ifdef WORDS_BIGENDIAN
  swap_octet(b[0], b[1]);
#endif
  return adv_idio_write(doc, offset, sizeof(int16), b);
}

int32
adv_idio_write_int16v (AdvIDocument* doc, adv_off_t offset, int num, const int16* val)
{
  int i;
  adv_off_t o = 0;
  for (i = 0; i < num; i++)
    o += adv_idio_write_int16(doc, offset+o, val[i]);
  return o;
}

int32
adv_idio_write_int32 (AdvIDocument* doc, adv_off_t offset, int32 val)
{
  octet* b;
  b = (octet*)&val;
#ifdef WORDS_BIGENDIAN
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return adv_idio_write(doc, offset, sizeof(int32), b);
}

int32
adv_idio_write_int32v (AdvIDocument* doc, adv_off_t offset, int num, const int32* val)
{
  int i;
  adv_off_t o = 0;
  for (i = 0; i < num; i++)
    o += adv_idio_write_int32(doc, offset+o, val[i]);
  return o;
}

int32
adv_idio_write_int64 (AdvIDocument* doc, adv_off_t offset, int64 val)
{
  octet* b;
  b = (octet*)&val;
#ifdef WORDS_BIGENDIAN
  swap_octet(b[0], b[7]);
  swap_octet(b[1], b[6]);
  swap_octet(b[2], b[5]);
  swap_octet(b[3], b[4]);
#endif
  return adv_idio_write(doc, offset, sizeof(int64), b);
}

int32
adv_idio_write_int64v (AdvIDocument* doc, adv_off_t offset, int num, const int64* val)
{
  int i;
  adv_off_t o = 0;
  for (i = 0; i < num; i++)
    o += adv_idio_write_int64(doc, offset+o, val[i]);
  return o;
}

int32
adv_idio_write_float32 (AdvIDocument* doc, adv_off_t offset, float32 val)
{
  octet* b;
  b = (octet*)&val;
#ifdef WORDS_BIGENDIAN
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return adv_idio_write(doc, offset, sizeof(float32), b);
}

int32
adv_idio_write_float32v (AdvIDocument* doc, adv_off_t offset, int num, const float32* val)
{
  int i;
  adv_off_t o = 0;
  for (i = 0; i < num; i++)
    o += adv_idio_write_float32(doc, offset+o, val[i]);
  return o;
}

int32
adv_idio_write_float64 (AdvIDocument* doc, adv_off_t offset, float64 val)
{
  octet* b;
  b = (octet*)&val;
#ifdef WORDS_BIGENDIAN
  swap_octet(b[0], b[7]);
  swap_octet(b[1], b[6]);
  swap_octet(b[2], b[5]);
  swap_octet(b[3], b[4]);
#endif
  return adv_idio_write(doc, offset, sizeof(float64), b);
}

int32
adv_idio_write_float64v (AdvIDocument* doc, adv_off_t offset, int num, const float64* val)
{
  int i;
  adv_off_t o = 0;
  for (i = 0; i < num; i++)
    o += adv_idio_write_float64(doc, offset+o, val[i]);
  return o;
}

#endif
