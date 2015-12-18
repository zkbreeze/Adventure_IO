/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include "advsys-config.h"
#include <Adv/AdvUtil.h>
#include "AdvFDocument.h"
#include "AdvDocument.h"
#include <string.h>
#include <sys/param.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

/*============================================================
  ADV datafile access (declare)
  ============================================================*/

static const int sizeof_adv_off_t = 8;

struct _AdvFDocFile
{
  AdvFile* file;
  char* filepath;
  AList* documents;
  ATree* doctree;
};

struct _AdvFDocument
{
  struct _AdvFDocFile* dfile;
  bool close_dfile;				  /* close file and free this struct when this
                                                     document is closed. (true in case opened by
                                                     locator, false in other. if false, this
						     struct willbe freed when the file
						     containing this document is closed.) */
  bool closed;
  char* documentid;				  /* document id */
  char* locator;				  /* locator */
  AList* registry;				  /* registry */
  adv_off_t magicstring_head;			  /* offset of the magic string(confirm offset points correct document) */
  adv_off_t content_size;
  adv_off_t content_size_head;			  /* offset of the content_size field */
  adv_off_t content_head;			  /* offset of the content */
  adv_off_t registry_size;
  adv_off_t registry_size_head;			  /* offset of the registry_size field */
  adv_off_t registry_head;			  /* offset of the registry */
  bool dirty;
};

typedef struct _Property
{
  char* key;
  char* val;
} Property;

static void adv_fdio_free(AdvFDocument* doc);
static AdvFDocument* adv_fdio_open_by_offset(AdvFDocFile* dfile, adv_off_t offset);
static const char magicstring[] = "AdvDoc";
static const char magicstring_asc[] = "AdvDoc/ascii";

/*============================================================
  tree (define)
  ============================================================*/

static int cmp_did(void* va, void* vb)
{
  const char* p;
  unsigned int ha, hb;
  const char* a = (const char*)va;
  const char* b = (const char*)vb;

  ha = *a;
  if (ha)
    for (p = a+1; *p; p++)
      ha = (ha << 5) - ha + *p;

  hb = *b;
  if (hb)
    for (p = b+1; *p; p++)
      hb = (hb << 5) - hb + *p;
  
  if (ha == hb)
    return strcmp(a, b);
  else
    return ha - hb;
}

/*
static void _a_tree_node_print(ATreeNode* node, int level)
{
  if (node->left)
    a_tree_node_print(node->left, level+1);
  printf("%d %s\n", level, ((AdvFDocument*)node->data)->documentid);
  if (node->right)
    a_tree_node_print(node->right, level+1);
}
*/

/*============================================================
  ADV datafile access (define)
  ============================================================*/

/*------------------------------------------------------------
  adv_fdio_file_create
  - creates a new file
  ------------------------------------------------------------*/

static
AdvFDocFile*
adv_fdio_file_create(const char* filename)
{
  AdvFDocFile* dfile;
  AdvFile* file;

  file = adv_fio_create(filename);
  if (!file)
    return NULL;

  dfile = a_new(AdvFDocFile);
#if 0
  dfile->file = adv_fio_create(filename);
#endif
  dfile->file = file;
  dfile->filepath = a_new_array(char, MAXPATHLEN);
#ifdef HAVE_FUNC_REALPATH
  realpath(filename, dfile->filepath);
#else
  strcpy(dfile->filepath, filename);
#endif
  dfile->documents = a_list_new();
  dfile->doctree = a_tree_new(cmp_did);
  return dfile;
}

/*------------------------------------------------------------
  adv_fdio_file_append
  - opens a existing file for append
  ------------------------------------------------------------*/

static
AdvFDocFile*
adv_fdio_file_append(const char* filename)
{
  AdvFDocFile* dfile;
  AdvFDocument* doc;
  adv_off_t off;
  AdvFile* file;

  file = adv_fio_open(filename, "w");
  if (!file)
    return NULL;

  dfile = a_new(AdvFDocFile);
  dfile->file = file;
  dfile->filepath = a_new_array(char, MAXPATHLEN);
#ifdef HAVE_FUNC_REALPATH
  realpath(filename, dfile->filepath);
#else
  strcpy(dfile->filepath, filename);
#endif
  dfile->documents = a_list_new();
  dfile->doctree = a_tree_new(cmp_did);
  
  /* parse existing document */
  off = 0;
  while (off < adv_fio_get_size(dfile->file))
    {
      doc = adv_fdio_open_by_offset(dfile, off);
      if (!doc)
	{
	  /* adv_fdio_file_close(dfile); */
	  /* return NULL; */
	  break;
	}
      dfile->documents = a_list_prepend(dfile->documents, doc);
      off += sizeof(magicstring) + doc->content_size + doc->registry_size + sizeof_adv_off_t*2;
    }
  return dfile;
}

/*------------------------------------------------------------
  adv_fdio_file_open_for_read
  - opens a existing file for read (read only open)
  ------------------------------------------------------------*/

static
AdvFDocFile*
adv_fdio_file_open_for_read(const char* filename)
{
  AdvFDocFile* dfile;
  AdvFDocument* doc;
  adv_off_t off;

#ifdef DEBUG
  fprintf (stdout, "called: adv_fdio_file_open_for_read()\n");
  fprintf (stdout, "  filename=[%s]\n", filename);
#endif

  dfile = a_new(AdvFDocFile);
  dfile->file = adv_fio_open(filename, "r");
  dfile->filepath = a_new_array(char, MAXPATHLEN);
#ifdef HAVE_FUNC_REALPATH
  realpath(filename, dfile->filepath);
#else
  strcpy(dfile->filepath, filename);
#endif
  if (dfile->file == NULL)
    {
      a_free(dfile);
      return NULL;
    }
  dfile->documents = a_list_new();
  dfile->doctree = a_tree_new(cmp_did);
  
  /* parse existing document */
  off = 0;
  while (off < adv_fio_get_size(dfile->file))
    {
      doc = adv_fdio_open_by_offset(dfile, off);

#ifdef DEBUG
      if (doc == NULL) {
	fprintf (stdout, "off=%zd: doc=NULL\n", off);
      } else {
	fprintf (stdout, "off=%zd: doc=[%s]\n", off, adv_fdio_get_documentid(doc));
      }
#endif

      if (!doc)
	{
	  adv_fdio_file_close(dfile);
	  return NULL;
	}
      dfile->documents = a_list_prepend(dfile->documents, doc);
      dfile->doctree = a_tree_insert(dfile->doctree, doc, (void*)adv_fdio_get_documentid(doc));

      off += (adv_off_t)sizeof(magicstring) + (adv_off_t)doc->content_size + (adv_off_t)doc->registry_size + (adv_off_t)(sizeof_adv_off_t*2);

#ifdef DEBUG
      fprintf (stdout, "  sizeof(magicstring)=%zd\n", sizeof(magicstring));
      fprintf (stdout, "  doc->content_size=%zd\n", doc->content_size);
      fprintf (stdout, "  doc->registry_size=%zd\n", doc->registry_size);
      fprintf (stdout, "  off->%zd: total size=%zd\n", off, adv_fio_get_size (dfile->file));
#endif

    }
  return dfile;
}

/*------------------------------------------------------------
  adv_fdio_file_open
  ------------------------------------------------------------*/

AdvFDocFile*
adv_fdio_file_open(const char* filename, const char* mode)
{
  struct stat statbuf;
  switch (tolower(mode[0]))
    {
    case 'a':
      if (stat(filename, &statbuf) == -1 && errno == ENOENT)
	return adv_fdio_file_create(filename);
      else
	return adv_fdio_file_append(filename);
    case 'c':
      return adv_fdio_file_create(filename);
    case 'r':
      return adv_fdio_file_open_for_read(filename);
    }
  return NULL;
}


/*------------------------------------------------------------
  adv_fdio_file_close
  - closes a file and all documents in the file
  ------------------------------------------------------------*/

void
adv_fdio_file_close(AdvFDocFile* dfile)
{
  AList* i;
  for (i = dfile->documents; i != NULL; i = i->next)
    {
      AdvFDocument* doc = (AdvFDocument*)i->data;
      if (!doc->closed)
	adv_fdio_close(doc);
      adv_fdio_free(doc);
    }
  a_list_destroy(dfile->documents);
  a_tree_destroy(dfile->doctree);
  adv_fio_close(dfile->file);

  a_free(dfile->filepath);
  a_free(dfile);
}

/*------------------------------------------------------------
  adv_fdio_file_get_locator
  ------------------------------------------------------------*/

const char*
adv_fdio_file_get_locator(AdvFDocFile* dfile)
{
  static char* buf = NULL;
  free(buf);
  buf = (char*)malloc(sizeof(char)*(strlen(dfile->filepath)+20));
  sprintf(buf, "advdoc:%s", dfile->filepath);
  return buf;
}

/*============================================================
  ============================================================*/

static
adv_off_t
read_line(AdvFile* file, adv_off_t offset, int max, char* buf)
{
  int i;
  i = 0;
  while (i < max)
    {
      adv_fio_read(file, offset+i, 1, buf+i);
      if (buf[i] == '\n')
	{
	  buf[i] = '\0';			  /* chop! */
	  break;
	}
      i++;
    }
  if (i >= 1 && buf[i-1] == '\r')		  /* CR */
    buf[i-1] = '\0';				  /* chop! */
  return i+1;
}

static
Property*
parse_line(const char* line)
{
  Property* prop;
  const char* key;
  int key_len;
  const char* val;
  int val_len;
  const char* p;
  const char* equal;

  equal = strchr(line, '=');
  if (!equal)
    return NULL;

  p = line;
  if ( (*p == ' ') || (*p == '\t') )
    p++;
  key = p;
  p = equal;
  while ((p-1 > key) && ( (*(p-1) == ' ') || (*(p-1) == '\t') ) )
    p--;
  key_len = p - key;
  
  p = equal+1;
  if ( (*p == ' ') || (*p == '\t') )
    p++;
  val = p;
  p = line+strlen(line);
  while ((p-1 > key) && ( (*(p-1) == ' ') || (*(p-1) == '\t') ) )
    p--;
  val_len = p - val;

  if (key_len <= 0)
    return NULL;

  prop = a_new(Property);
  prop->key = a_new_array(char, key_len+1);
  memcpy(prop->key, key, key_len);
  prop->key[key_len] = '\0';
  prop->val = a_new_array(char, val_len+1);
  memcpy(prop->val, val, val_len);
  prop->val[val_len] = '\0';
  return prop;
}

/*------------------------------------------------------------
  adv_fdio_open_by_offset
  - opens a document exists at specified file and offset
  - only this function does real procedure
  - this func. willbe called by adv_fdio_file_{open|append}
  ------------------------------------------------------------*/

static
AdvFDocument*
adv_fdio_open_by_offset(AdvFDocFile* dfile, adv_off_t offset)
{
  AdvFDocument* doc;
  adv_off_t off;
  char buf[2048];
  int32 num_props;
  int i;

#ifdef DEBUG
  fprintf(stdout, "adv_fdio_open_by_offset(): start (offset=%zd)\n", offset);
#endif

  /* check whether the offset points correct document */
  char magic[sizeof(magicstring)+sizeof(magicstring_asc)];
  adv_fio_read(dfile->file, offset, sizeof(magicstring)-1, magic);
  if (memcmp(magicstring, magic, sizeof(magicstring)-1) != 0)
    return NULL;

  /* check whether the document is ascii document */
  adv_fio_read(dfile->file, offset, sizeof(magicstring_asc)-1, magic);
  if (memcmp(magicstring_asc, magic, sizeof(magicstring_asc)-1) == 0)
    {
      Property* prop;

      /* ascii doc */
      off = offset;
      doc = a_new(AdvFDocument);
      doc->dirty = false;
      doc->dfile = dfile;
      doc->close_dfile = false;
      doc->closed = true;
      doc->magicstring_head = offset;
      doc->content_size_head = -1;
      doc->content_head = -1;
      doc->content_size = 0;
      doc->registry_size_head = -1;
      doc->registry_head = -1;
      doc->registry_size = adv_fio_get_size(dfile->file)-(sizeof(magicstring) + sizeof_adv_off_t*2 + off);
      off += read_line(dfile->file, off, sizeof(buf), buf);
      doc->registry_head = off;

      doc->documentid = NULL;
      doc->locator = a_new_array(char, strlen(dfile->filepath)+1);
      strcpy(doc->locator, dfile->filepath);
      doc->registry = a_list_new();
      
      while (true)
	{
	  off += read_line(dfile->file, off, sizeof(buf), buf);
	  prop = parse_line(buf);
	  if (!prop)
	    break;
	  doc->registry = a_list_prepend(doc->registry, prop);
	}
    }
  else
    {
      doc = a_new(AdvFDocument);
      doc->dirty = false;
      doc->dfile = dfile;
      doc->close_dfile = false;
      doc->closed = true;
      doc->magicstring_head = offset;
      offset += sizeof(magicstring);
      doc->content_size_head = offset;
      doc->content_head = offset + sizeof_adv_off_t;


#ifdef DEBUG
      fprintf(stdout, "adv_fdio_open_by_offset(): content_size_head=%zd\n", doc->content_size_head);
#endif


      //adv_fio_read_int64(dfile->file, doc->content_size_head, &(doc->content_size));
      {
	adv_off_t csize;
	adv_fio_read_int64(dfile->file, doc->content_size_head, &csize);

#ifdef DEBUG
      fprintf(stdout, "adv_fdio_open_by_offset(): csize=%zd\n", csize);
#endif

	doc->content_size = csize;
      }
      doc->registry_size_head = doc->content_head + doc->content_size;


#ifdef DEBUG
      fprintf(stdout, "adv_fdio_open_by_offset(): content_size=%zd\n", doc->content_size);
      fprintf(stdout, "adv_fdio_open_by_offset(): registry_size_head=%zd\n", doc->registry_size_head);
#endif




      doc->registry_head = doc->registry_size_head + sizeof_adv_off_t;
      adv_fio_read_int64(dfile->file, doc->registry_size_head, &(doc->registry_size));

      off = doc->registry_head;
      doc->documentid = a_new_array(char, adv_fio_read_string_length(dfile->file, off)+1);
      off += adv_fio_read_string(dfile->file, off, doc->documentid);
      doc->locator = a_new_array(char, strlen(dfile->filepath)+strlen(doc->documentid)+5);
      strcpy(doc->locator, dfile->filepath);
      strcat(doc->locator, "?");
      strcat(doc->locator, doc->documentid);
  
      /*
	printf("\nID: %s\n", doc->documentid);
	printf("content-size: %d\n", (int32)doc->content_size);
	printf("registry-size: %d\n", (int32)doc->registry_size);
      */
#ifdef DEBUG
      fprintf(stdout, "\nID: %s\n", doc->documentid);
      fprintf(stdout, "content-size: %zd\n", doc->content_size);
      fprintf(stdout, "registry-size: %zd\n", doc->registry_size);
      fprintf (stdout, "  (sizeof(adv_off_t)=%zd)\n", sizeof (adv_off_t));
#endif
  
      off += adv_fio_read_int32(dfile->file, off, &num_props);

#ifdef DEBUG
      fprintf(stdout, "num_props=%d\n", num_props);
#endif


      doc->registry = a_list_new();
      for (i = 0; i < num_props; i++)
	{
	  Property* prop = a_new(Property);
	  prop->key = a_new_array(char, adv_fio_read_string_length(dfile->file, off)+1);
	  off += adv_fio_read_string(dfile->file, off, prop->key);
	  prop->val = a_new_array(char, adv_fio_read_string_length(dfile->file, off)+1);
	  off += adv_fio_read_string(dfile->file, off, prop->val);
	  /* printf("%s=%s\n", prop->key, prop->val); */

#ifdef DEBUG
	  fprintf(stdout, "%s=%s\n", prop->key, prop->val);
#endif

	  doc->registry = a_list_prepend(doc->registry, prop);
	}
    }
  
  return doc;
}

AdvFDocument*
adv_fdio_create(AdvFDocFile* dfile, const char* did)
{
  AdvFDocument* doc;
  if (!did)
    did = adv_dio_make_documentid("Doc");
  doc = a_new(AdvFDocument);
  doc->dirty = true;
  doc->dfile = dfile;
  doc->close_dfile = false;
  doc->closed = false;
  doc->documentid = a_new_array(char, strlen(did)+1);
  strcpy(doc->documentid, did);
  doc->locator = a_new_array(char, strlen(dfile->filepath)+strlen(doc->documentid)+5);
  strcpy(doc->locator, dfile->filepath);
  strcat(doc->locator, "?");
  strcat(doc->locator, doc->documentid);
  doc->magicstring_head = adv_fio_get_size(doc->dfile->file);
  doc->content_size_head = doc->magicstring_head + sizeof(magicstring);
  doc->content_head = doc->content_size_head + sizeof_adv_off_t;
  doc->registry_size_head = -1;
  doc->registry_head = -1;
  doc->registry = a_list_new();
  dfile->documents = a_list_prepend(dfile->documents, doc);
  return doc;
}

AdvFDocument*
adv_fdio_open_nth(AdvFDocFile* dfile, int n)
{
  AList* l;
  for (l = dfile->documents; l != NULL; l = l->next, n--)
    {
      if (n == 0)
	return (AdvFDocument*)l->data;
    }
  return NULL;
}

AdvFDocument*
adv_fdio_open_by_documentid(AdvFDocFile* dfile, const char* did)
{
#if 0
  AList* l;
  for (l = dfile->documents; l != NULL; l = l->next)
    {
      AdvFDocument* doc = (AdvFDocument*)l->data;
      if (strcmp(doc->documentid, did) == 0)
	return doc;
    }
#else
  AdvFDocument* doc;
  doc = a_tree_lookup(dfile->doctree, (void*)did);
  if (doc)
    return doc;
#endif
  return NULL;
}

AdvFDocument*
adv_fdio_open_by_property(AdvFDocFile* dfile, void* prev, ...)
{
  AList* l;
  bool b = false;
  const char* key;
  const char* val;
  const char* v;

  if (prev == NULL)
    b = true;
  for (l = dfile->documents; l != NULL; l = l->next)
    {
      AdvFDocument* doc = (AdvFDocument*)l->data;
      va_list va;
      va_start(va, prev);
      while (true)
	{
	  key = va_arg(va, const char*);
	  val = va_arg(va, const char*);
	  if (key == NULL)
	    break;
	  v = adv_fdio_get_property(doc, key);
	  if (v && strcmp(v, val) != 0)
	    break;
	}
      va_end(va);
      if (key == NULL)
	{
	  if (b)
	    return doc;
	  else if (doc == (AdvFDocument *) prev)
	    b = true;
	}
    }
  return NULL;
}

AdvFDocument*
adv_fdio_open_by_property_v(AdvFDocFile* dfile, void* prev, va_list va)
{
  AList* l;
  bool b = false;
  const char* key;
  const char* val;
  const char* v;

  if (prev == NULL)
    b = true;
  for (l = dfile->documents; l != NULL; l = l->next)
    {
      AdvFDocument* doc = (AdvFDocument*)l->data;
      va_list va2;
      va_copy(va2, va);
      while (true)
	{
	  key = va_arg(va2, const char*);
	  val = va_arg(va2, const char*);
	  if (key == NULL)
	    break;
	  v = adv_fdio_get_property(doc, key);
	  if (v && strcmp(v, val) != 0)
	    break;
	}
      va_end(va2);
      if (key == NULL)
	{
	  if (b)
	    return doc;
	  else if (doc == (AdvFDocument *) prev)
	    b = true;
	}
    }
  return NULL;
}

AdvFDocument*
adv_fdio_open_by_locator(const char* locator)
{
  const char* p;
  if (strncasecmp(locator, "advdoc:", 7) == 0)
    locator += 7;

  if ((p = strchr(locator, '?')) != NULL)
    {
      AdvFDocFile* dfile;
      AdvFDocument* fdoc;

      /* single file document */
      char* filename = a_new_array(char, p - locator + 1);
      const char* did = p+1;
      memcpy(filename, locator, p-locator);
      filename[p-locator] = '\0';
      
      /* filename: filename, did: documentid */
      dfile = adv_fdio_file_open(filename, "r");
      a_free(filename);
      if (dfile)
	{
	  fdoc = adv_fdio_open_by_documentid(dfile, did);
	  if (fdoc)
	    {
	      fdoc->close_dfile = true;
	      return fdoc;
	    }
	  adv_fdio_file_close(dfile);
	}
    }
  else
    {
      AdvFDocFile* dfile;
      AdvFDocument* fdoc;

      dfile = adv_fdio_file_open(locator, "r");
      if (dfile)
	{
	  if (a_list_length(dfile->documents) == 1)
	    {
	      fdoc = adv_fdio_open_nth(dfile, 0);
	      fdoc->close_dfile = true;
	      return fdoc;
	    }
	  adv_fdio_file_close(dfile);
	}
    }
  return NULL;
}

void
adv_fdio_close(AdvFDocument* doc)
{
  adv_off_t length;
  AdvFile* file;

#ifdef DEBUG
  fprintf (stdout, "adv_fdio_close(): doc->dirty=%d\n", doc->dirty);
#endif

  if (doc->dirty)
    {
      file = doc->dfile->file;

      /* write magicstring */
      adv_fio_write(file, doc->magicstring_head, sizeof(magicstring), magicstring);

      adv_fio_write_int64(file, doc->content_size_head, 0);
      length = adv_fio_get_size(file) - doc->content_head;
      adv_fio_write_int64(file, doc->content_size_head, length);

#ifdef DEBUG
      fprintf (stdout, "adv_fdio_close(): doc->documentid=[%s]\n", doc->documentid);
      fprintf (stdout, "adv_fdio_close(): doc->content_head=%zd\n", doc->content_head);
      fprintf (stdout, "adv_fdio_close(): advfio_get_size(file)=%zd\n", adv_fio_get_size (file));
      fprintf (stdout, "adv_fdio_close(): length=%zd\n", length);
#endif


      /* write registry */
      doc->registry_size_head = adv_fio_get_size(file);
      doc->registry_head = doc->registry_size_head+sizeof_adv_off_t;
      {
	AList* l;
	adv_off_t off;
	off = doc->registry_head;
	off += adv_fio_write_string(file, off, doc->documentid);
	off += adv_fio_write_int32(file, off, a_list_length(doc->registry));
	for (l = doc->registry; l != NULL; l = l->next)
	  {
	    Property* prop = (Property*)l->data;
	    off += adv_fio_write_string(file, off, prop->key);
	    off += adv_fio_write_string(file, off, prop->val);
	  }
#ifdef HAVE_ADVSYS_INT64
	adv_fio_write_int64(file,
			    doc->registry_size_head,
			    (int64)off - doc->registry_head);
#else
	adv_fio_write_int64(file,
			    doc->registry_size_head,
			    off - doc->registry_head);
#endif
	doc->dirty = false;
      }
    }
  doc->closed = true;

  if (doc->close_dfile)
    {
      adv_fdio_file_close(doc->dfile);		  /* free of this document structure
						     willbe done while adv_fdio_close() */
    }
}

static
void
adv_fdio_free(AdvFDocument* doc)
{
  AList* l;
  a_free(doc->documentid);
  a_free(doc->locator);
  for (l = doc->registry; l != NULL; l = l->next)
    {
      Property* p = (Property*)l->data;
      a_free(p->key);
      a_free(p->val);
      a_free(p);
    }
  a_list_destroy(doc->registry);
  a_free(doc);
}

/*------------------------------------------------------------*/

void
adv_fdio_set_property(AdvFDocument* doc, const char* key, const char* val)
{
  Property* prop;
  AList* i;
  doc->dirty = true;
  for (i = doc->registry; i != NULL; i = i->next)
    {
      prop = (Property*)i->data;
      if (strcmp(prop->key, key) == 0)
	{
	  a_free(prop->val);
	  prop->val = a_new_array(char, strlen(val)+1);
	  strcpy(prop->val, val);
	  return;
	}
    }
  prop = a_new(Property);
  prop->key = a_new_array(char, strlen(key)+1);
  strcpy(prop->key, key);
  prop->val = a_new_array(char, strlen(val)+1);
  strcpy(prop->val, val);
  doc->registry = a_list_prepend(doc->registry, prop);
}

void
adv_fdio_unset_nth_property(AdvFDocument* doc, int n)
{
  AList* l = doc->registry;
  int i;
  for (i = 0; i < n && l; i++)
    l = l->next;
  if (l)
    {
      Property* p = (Property*)l->data;
      a_free(p->key);
      a_free(p->val);
      a_free(p);
      doc->registry = a_list_remove(l);
    }
}

void
adv_fdio_set_property_int32(AdvFDocument* doc,
			   const char* key,
			   int32 val)
{
  char buf[100];
  sprintf(buf, "%d", val);
  adv_fdio_set_property(doc, key, buf);
}

void
adv_fdio_set_property_float64(AdvFDocument* doc,
			     const char* key,
			     float64 val)
{
  char buf[100];
  sprintf(buf, "%g", val);
  adv_fdio_set_property(doc, key, buf);
}

const char*
adv_fdio_get_property(AdvFDocument* doc,
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

bool
adv_fdio_get_nth_property(AdvFDocument* doc,
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
adv_fdio_get_property_int32(AdvFDocument* doc,
			   const char* key,
			   int32* val)
{
  const char* v = adv_fdio_get_property(doc, key);
  if (v)
    {
      *val = atoi(v);
      return true;
    }
  else
    return false;
}

bool
adv_fdio_get_property_float64(AdvFDocument* doc,
			     const char* key,
			     float64* val)
{
  const char* v = adv_fdio_get_property(doc, key);
  if (v)
    {
      *val = strtod(v, NULL);
      return true;
    }
  else
    return false;
}

bool
adv_fdio_check_property_v(AdvFDocument* doc, va_list va)
{
  const char* key;
  const char* val;
  const char* v;

  while (true)
    {
      key = va_arg(va, const char*);
      if (key == NULL)
	break;
      val = va_arg(va, const char*);
      v = adv_fdio_get_property(doc, key);
      if (!v || strcmp(v, val) != 0)
	break;
    }
  if (key == NULL)
    return true;
  else
    return false;
}

bool
adv_fdio_check_property(AdvFDocument* doc, ...)
{
  va_list va;
  bool ret;
  va_start(va, doc);
  ret = adv_fdio_check_property_v(doc, va);
  va_end(va);
  return ret;
}

/*------------------------------------------------------------*/

const char*
adv_fdio_get_documentid(AdvFDocument* doc)
{
  return doc->documentid;
}

const char*
adv_fdio_get_locator(AdvFDocument* doc)
{
  return doc->locator;
}

adv_off_t
adv_fdio_get_size(AdvFDocument* doc)
{
  return doc->content_size;
}

/*------------------------------------------------------------*/

int32
adv_fdio_write_octet(AdvFDocument* doc, adv_off_t offset, int32 length, const octet* buf)
{
  doc->dirty = true;
  return adv_fio_write(doc->dfile->file, doc->content_head+offset, length, buf);
}

int32
adv_fdio_write_string(AdvFDocument* doc, adv_off_t offset, const char* buf)
{
  int32 length = strlen(buf);
  doc->dirty = true;
  adv_fdio_write_int32(doc, offset, length);
  return adv_fdio_write_octet(doc, offset+sizeof(int32), length, 
                               (const octet *) buf) + sizeof(int32);
}

int32
adv_fdio_write_int8(AdvFDocument* doc, adv_off_t offset, int8 val)
{
  doc->dirty = true;
  return adv_fio_write_int8(doc->dfile->file, doc->content_head+offset, val);
}

int32
adv_fdio_write_int8v(AdvFDocument* doc, adv_off_t offset, int num, const int8* val)
{
  int i, off;
  doc->dirty = true;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_write_int8(doc->dfile->file, doc->content_head+offset+off, val[i]);
  return off;
}

int32
adv_fdio_write_int16(AdvFDocument* doc, adv_off_t offset, int16 val)
{
  doc->dirty = true;
  return adv_fio_write_int16(doc->dfile->file, doc->content_head+offset, val);
}

int32
adv_fdio_write_int16v(AdvFDocument* doc, adv_off_t offset, int num, const int16* val)
{
  int i, off;
  doc->dirty = true;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_write_int16(doc->dfile->file, doc->content_head+offset+off, val[i]);
  return off;
}

int32
adv_fdio_write_int32(AdvFDocument* doc, adv_off_t offset, int32 val)
{
  doc->dirty = true;
  return adv_fio_write_int32(doc->dfile->file, doc->content_head+offset, val);
}

int32
adv_fdio_write_int32v(AdvFDocument* doc, adv_off_t offset, int num, const int32* val)
{
  int i, off;
  doc->dirty = true;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_write_int32(doc->dfile->file, doc->content_head+offset+off, val[i]);
  return off;
}

#ifdef HAVE_ADVSYS_INT64
int32
adv_fdio_write_int64(AdvFDocument* doc, adv_off_t offset, int64 val)
{
  doc->dirty = true;
  return adv_fio_write_int64(doc->dfile->file, doc->content_head+offset, val);
}
int32
adv_fdio_write_int64v(AdvFDocument* doc, adv_off_t offset, int num, const int64* val)
{
  int i, off;
  doc->dirty = true;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_write_int64(doc->dfile->file, doc->content_head+offset+off, val[i]);
  return off;
}
#else
int32
adv_fdio_write_int64(AdvFDocument* doc, adv_off_t offset, int32 val)
{
  doc->dirty = true;
  return adv_fio_write_int64(doc->dfile->file, doc->content_head+offset, val);
}
int32
adv_fdio_write_int64v(AdvFDocument* doc, adv_off_t offset, int num, const int32* val)
{
  int i, off;
  doc->dirty = true;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_write_int64(doc->dfile->file, doc->content_head+offset+off, val[i]);
  return off;
}
#endif

int32
adv_fdio_write_float32(AdvFDocument* doc, adv_off_t offset, float32 val)
{
  doc->dirty = true;
  return adv_fio_write_float32(doc->dfile->file, doc->content_head+offset, val);
}
int32
adv_fdio_write_float32v(AdvFDocument* doc, adv_off_t offset, int num, const float32* val)
{
  int i, off;
  doc->dirty = true;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_write_float32(doc->dfile->file, doc->content_head+offset+off, val[i]);
  return off;
}

int32
adv_fdio_write_float64(AdvFDocument* doc, adv_off_t offset, float64 val)
{
  doc->dirty = true;
  return adv_fio_write_float64(doc->dfile->file, doc->content_head+offset, val);
}
int32
adv_fdio_write_float64v(AdvFDocument* doc, adv_off_t offset, int num, const float64* val)
{
  int i, off;
  doc->dirty = true;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_write_float64(doc->dfile->file, doc->content_head+offset+off, val[i]);
  return off;
}

int32
adv_fdio_read_octet(AdvFDocument* doc, adv_off_t offset, int32 length, octet* buf)
{
  return adv_fio_read(doc->dfile->file, doc->content_head+offset, length, buf);
}

int32
adv_fdio_read_string_length(AdvFDocument* doc, adv_off_t offset)
{
  int32 length;
  adv_fdio_read_int32(doc, offset, &length);
  return length;
}

int32
adv_fdio_read_string(AdvFDocument* doc, adv_off_t offset, char* buf)
{
  int32 length;
  int32 a = 0;
  a += adv_fdio_read_int32(doc, offset, &length);
  a += adv_fdio_read_octet(doc, offset+a, length, (octet *) buf);
  buf[length] = '\0';
  return a;
}

int32
adv_fdio_read_int8(AdvFDocument* doc, adv_off_t offset, int8* val)
{
  return adv_fio_read_int8(doc->dfile->file, doc->content_head+offset, val);
}

int32
adv_fdio_read_int8v(AdvFDocument* doc, adv_off_t offset, int num, int8* val)
{
  int i, off;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_read_int8(doc->dfile->file, doc->content_head+offset+off, val+i);
  return off;
}

int32
adv_fdio_read_int16(AdvFDocument* doc, adv_off_t offset, int16* val)
{
  return adv_fio_read_int16(doc->dfile->file, doc->content_head+offset, val);
}

int32
adv_fdio_read_int16v(AdvFDocument* doc, adv_off_t offset, int num, int16* val)
{
  int i, off;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_read_int16(doc->dfile->file, doc->content_head+offset+off, val+i);
  return off;
}

int32
adv_fdio_read_int32(AdvFDocument* doc, adv_off_t offset, int32* val)
{
  return adv_fio_read_int32(doc->dfile->file, doc->content_head+offset, val);
}

int32
adv_fdio_read_int32v(AdvFDocument* doc, adv_off_t offset, int num, int32* val)
{
  int i, off;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_read_int32(doc->dfile->file, doc->content_head+offset+off, val+i);
  return off;
}

#ifdef HAVE_ADVSYS_INT64
int32
adv_fdio_read_int64(AdvFDocument* doc, adv_off_t offset, int64* val)
{
  return adv_fio_read_int64(doc->dfile->file, doc->content_head+offset, val);
}
int32
adv_fdio_read_int64v(AdvFDocument* doc, adv_off_t offset, int num, int64* val)
{
  int i, off;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_read_int64(doc->dfile->file, doc->content_head+offset+off, val+i);
  return off;
}
#else
int32
adv_fdio_read_int64(AdvFDocument* doc, adv_off_t offset, int32* val)
{
  return adv_fio_read_int64(doc->dfile->file, doc->content_head+offset, val);
}
int32
adv_fdio_read_int64v(AdvFDocument* doc, adv_off_t offset, int num, int32* val)
{
  int i, off;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_read_int64(doc->dfile->file, doc->content_head+offset+off, val+i);
  return off;
}
#endif

int32
adv_fdio_read_float32(AdvFDocument* doc, adv_off_t offset, float32* val)
{
  return adv_fio_read_float32(doc->dfile->file, doc->content_head+offset, val);
}
int32
adv_fdio_read_float32v(AdvFDocument* doc, adv_off_t offset, int num, float32* val)
{
  int i, off;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_read_float32(doc->dfile->file, doc->content_head+offset+off, val+i);
  return off;
}

int32
adv_fdio_read_float64(AdvFDocument* doc, adv_off_t offset, float64* val)
{
  return adv_fio_read_float64(doc->dfile->file, doc->content_head+offset, val);
}
int32
adv_fdio_read_float64v(AdvFDocument* doc, adv_off_t offset, int num, float64* val)
{
  int i, off;
  off = 0;
  for (i = 0; i < num; i++)
    off += adv_fio_read_float64(doc->dfile->file, doc->content_head+offset+off, val+i);
  return off;
}
