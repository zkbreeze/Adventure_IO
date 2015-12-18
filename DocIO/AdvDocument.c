/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include <Adv/AdvUtil.h>
#include "AdvDocument.h"
#include "AdvFDocument.h"
/* #include "AdvNDocument.h" */
#include "AdvIDocument.h"
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <time.h>

#include "AdvDocumentPrivate.h"

#define swap_octet(a, b) {octet c; c = a; a = b; b = c;}

/*======================================================================

  AdvDocFile

  ======================================================================*/

AdvDocFileFuncs fdfile_funcs =
{
  (DFILE_CONVERT_FUNC)            adv_dio_new_from_fdoc,
  (DFILE_FILE_OPEN_FUNC)          adv_fdio_file_open,
  (DFILE_FILE_CLOSE_FUNC)         adv_fdio_file_close,
  (DFILE_CREATE_FUNC)             adv_fdio_create,
  (DFILE_OPEN_BY_DOCUMENTID_FUNC) adv_fdio_open_by_documentid,
  (DFILE_OPEN_NTH_FUNC)           adv_fdio_open_nth,
  (DFILE_OPEN_BY_PROPERTY_V_FUNC) adv_fdio_open_by_property_v,
  (DFILE_GET_LOCATOR_FUNC)        adv_fdio_file_get_locator,
};

#ifdef HAVE_ORBIT
AdvDocFileFuncs idfile_funcs =
{
  (DFILE_CONVERT_FUNC)            adv_dio_new_from_idoc,
  (DFILE_FILE_OPEN_FUNC)          adv_idio_file_open,
  (DFILE_FILE_CLOSE_FUNC)         adv_idio_file_close,
  (DFILE_CREATE_FUNC)             adv_idio_create,
  (DFILE_OPEN_BY_DOCUMENTID_FUNC) adv_idio_open_by_documentid,
  (DFILE_OPEN_NTH_FUNC)           adv_idio_open_nth,
  (DFILE_OPEN_BY_PROPERTY_V_FUNC) adv_idio_open_by_property_v,
  (DFILE_GET_LOCATOR_FUNC)        adv_idio_file_get_locator,
};
#endif

AdvDocFile*
adv_dio_file_new_from_idfile (AdvIDocFile* idfile)
{
#if HAVE_ORBIT
  if (idfile)
    {
      AdvDocFile* doc = a_new(AdvDocFile);
      doc->dfile = idfile;
      doc->f = &idfile_funcs;
      return doc;
    }
#else
  return NULL;
#endif
}

AdvDocFile*
adv_dio_file_open (const char* locator, const char* mode)
{
  if (strncasecmp(locator, "advdoc:", 7) == 0)
    locator += 7;

  if (strncasecmp(locator, "IOR:", 4) == 0)
    {
#if HAVE_ORBIT
      return adv_dio_file_new_from_idfile(adv_idio_file_open(locator, mode));
#else
      return NULL;
#endif
    }
  else 
    {
      AdvFDocFile* fdfile = adv_fdio_file_open(locator, mode);
      if (fdfile)
	{
	  AdvDocFile* doc = a_new(AdvDocFile);
	  doc->dfile = fdfile;
	  doc->f = &fdfile_funcs;
	  return doc;
	}
    }
  return NULL;
}

void
adv_dio_file_close (AdvDocFile* dfile)
{
  dfile->f->file_close(dfile->dfile);
  a_free(dfile);
}

AdvDocument*
adv_dio_create (AdvDocFile* dfile, const char* did)
{
  void* doc = dfile->f->create(dfile->dfile, did);
  AdvDocument* d = dfile->f->convert(doc);
  return d;
}

AdvDocument*
adv_dio_open_by_documentid (AdvDocFile* dfile, const char* did)
{
  void* doc = dfile->f->open_by_documentid(dfile->dfile, did);
  return dfile->f->convert(doc);
}

AdvDocument*
adv_dio_open_nth (AdvDocFile* dfile, int n)
{
  void* doc = dfile->f->open_nth(dfile->dfile, n);
  if (doc)
    return dfile->f->convert(doc);
  else
    return NULL;
}

AdvDocument*
adv_dio_open_by_property (AdvDocFile* dfile, void* prev, ...)
{
  void* doc;
  va_list va;
  va_start(va, prev);
  if(prev != NULL)
    {
      prev = ((AdvDocument*)prev)->doc;
    }
  doc = dfile->f->open_by_property_v(dfile->dfile, prev, va);
  va_end(va);
  return dfile->f->convert(doc);
}

const char*
adv_dio_file_get_locator (AdvDocFile* dfile)
{
  return dfile->f->get_locator(dfile->dfile);
}

/*======================================================================

  AdvDocument

  ======================================================================*/

AdvDocumentFuncs fdoc_funcs =
{
  (DOC_CLOSE_FUNC)                adv_fdio_close,

  (DOC_GET_DOCUMENTID_FUNC)       adv_fdio_get_documentid,
  (DOC_GET_SIZE_FUNC)             adv_fdio_get_size,
  (DOC_GET_LOCATOR_FUNC)          adv_fdio_get_locator,

  (DOC_GET_PROPERTY_FUNC)         adv_fdio_get_property,
  (DOC_GET_NTH_PROPERTY_FUNC)     adv_fdio_get_nth_property,
  (DOC_GET_PROPERTY_INT32_FUNC)   adv_fdio_get_property_int32,
  (DOC_GET_PROPERTY_FLOAT64_FUNC) adv_fdio_get_property_float64,
  (DOC_CHECK_PROPERTY_V_FUNC)     adv_fdio_check_property_v,

  (DOC_READ_OCTET_FUNC)           adv_fdio_read_octet,
  (DOC_READ_STRING_LENGTH_FUNC)   adv_fdio_read_string_length,
  (DOC_READ_STRING_FUNC)          adv_fdio_read_string,
  (DOC_READ_INT8_FUNC)            adv_fdio_read_int8,
  (DOC_READ_INT8V_FUNC)           adv_fdio_read_int8v,
  (DOC_READ_INT16_FUNC)           adv_fdio_read_int16,
  (DOC_READ_INT16V_FUNC)          adv_fdio_read_int16v,
  (DOC_READ_INT32_FUNC)           adv_fdio_read_int32,
  (DOC_READ_INT32V_FUNC)          adv_fdio_read_int32v,
  (DOC_READ_INT64_FUNC)           adv_fdio_read_int64,
  (DOC_READ_INT64V_FUNC)          adv_fdio_read_int64v,
  (DOC_READ_FLOAT32_FUNC)         adv_fdio_read_float32,
  (DOC_READ_FLOAT32V_FUNC)        adv_fdio_read_float32v,
  (DOC_READ_FLOAT64_FUNC)         adv_fdio_read_float64,
  (DOC_READ_FLOAT64V_FUNC)        adv_fdio_read_float64v,

  (DOC_SET_PROPERTY_FUNC)         adv_fdio_set_property,
  (DOC_SET_PROPERTY_INT32_FUNC)   adv_fdio_set_property_int32,
  (DOC_SET_PROPERTY_FLOAT64_FUNC) adv_fdio_set_property_float64,
  (DOC_UNSET_NTH_PROPERTY_FUNC)   adv_fdio_unset_nth_property,

  (DOC_WRITE_OCTET_FUNC)          adv_fdio_write_octet,
  (DOC_WRITE_STRING_FUNC)         adv_fdio_write_string,
  (DOC_WRITE_INT8_FUNC)           adv_fdio_write_int8,
  (DOC_WRITE_INT8V_FUNC)          adv_fdio_write_int8v,
  (DOC_WRITE_INT16_FUNC)          adv_fdio_write_int16,
  (DOC_WRITE_INT16V_FUNC)         adv_fdio_write_int16v,
  (DOC_WRITE_INT32_FUNC)          adv_fdio_write_int32,
  (DOC_WRITE_INT32V_FUNC)         adv_fdio_write_int32v,
  (DOC_WRITE_INT64_FUNC)          adv_fdio_write_int64,
  (DOC_WRITE_INT64V_FUNC)         adv_fdio_write_int64v,
  (DOC_WRITE_FLOAT32_FUNC)        adv_fdio_write_float32,
  (DOC_WRITE_FLOAT32V_FUNC)       adv_fdio_write_float32v,
  (DOC_WRITE_FLOAT64_FUNC)        adv_fdio_write_float64,
  (DOC_WRITE_FLOAT64V_FUNC)       adv_fdio_write_float64v,
};

#ifdef HAVE_ORBIT

AdvDocumentFuncs idoc_funcs =
{
  (DOC_CLOSE_FUNC)                adv_idio_close,

  (DOC_GET_DOCUMENTID_FUNC)       adv_idio_get_documentid,
  (DOC_GET_SIZE_FUNC)             adv_idio_get_size,
  (DOC_GET_LOCATOR_FUNC)          adv_idio_get_locator,

  (DOC_GET_PROPERTY_FUNC)         adv_idio_get_property,
  (DOC_GET_NTH_PROPERTY_FUNC)     adv_idio_get_nth_property,
  (DOC_GET_PROPERTY_INT32_FUNC)   adv_idio_get_property_int32,
  (DOC_GET_PROPERTY_FLOAT64_FUNC) adv_idio_get_property_float64,
  (DOC_CHECK_PROPERTY_V_FUNC)     adv_idio_check_property_v,

  (DOC_READ_OCTET_FUNC)           adv_idio_read_octet,
  (DOC_READ_STRING_LENGTH_FUNC)   adv_idio_read_string_length,
  (DOC_READ_STRING_FUNC)          adv_idio_read_string,
  (DOC_READ_INT8_FUNC)            adv_idio_read_int8,
  (DOC_READ_INT8V_FUNC)           adv_idio_read_int8v,
  (DOC_READ_INT16_FUNC)           adv_idio_read_int16,
  (DOC_READ_INT16V_FUNC)          adv_idio_read_int16v,
  (DOC_READ_INT32_FUNC)           adv_idio_read_int32,
  (DOC_READ_INT32V_FUNC)          adv_idio_read_int32v,
  (DOC_READ_INT64_FUNC)           adv_idio_read_int64,
  (DOC_READ_INT64V_FUNC)          adv_idio_read_int64v,
  (DOC_READ_FLOAT32_FUNC)         adv_idio_read_float32,
  (DOC_READ_FLOAT32V_FUNC)        adv_idio_read_float32v,
  (DOC_READ_FLOAT64_FUNC)         adv_idio_read_float64,
  (DOC_READ_FLOAT64V_FUNC)        adv_idio_read_float64v,

  (DOC_SET_PROPERTY_FUNC)         adv_idio_set_property,
  (DOC_SET_PROPERTY_INT32_FUNC)   adv_idio_set_property_int32,
  (DOC_SET_PROPERTY_FLOAT64_FUNC) adv_idio_set_property_float64,
  (DOC_UNSET_NTH_PROPERTY_FUNC)   adv_idio_unset_nth_property,

  (DOC_WRITE_OCTET_FUNC)          adv_idio_write_octet,
  (DOC_WRITE_STRING_FUNC)         adv_idio_write_string,
  (DOC_WRITE_INT8_FUNC)           adv_idio_write_int8,
  (DOC_WRITE_INT8V_FUNC)          adv_idio_write_int8v,
  (DOC_WRITE_INT16_FUNC)          adv_idio_write_int16,
  (DOC_WRITE_INT16V_FUNC)         adv_idio_write_int16v,
  (DOC_WRITE_INT32_FUNC)          adv_idio_write_int32,
  (DOC_WRITE_INT32V_FUNC)         adv_idio_write_int32v,
  (DOC_WRITE_INT64_FUNC)          adv_idio_write_int64,
  (DOC_WRITE_INT64V_FUNC)         adv_idio_write_int64v,
  (DOC_WRITE_FLOAT32_FUNC)        adv_idio_write_float32,
  (DOC_WRITE_FLOAT32V_FUNC)       adv_idio_write_float32v,
  (DOC_WRITE_FLOAT64_FUNC)        adv_idio_write_float64,
  (DOC_WRITE_FLOAT64V_FUNC)       adv_idio_write_float64v,
#if 0
  adv_ndio_close,

  adv_ndio_get_documentid,
  adv_ndio_get_size,
  adv_ndio_get_locator,

  adv_ndio_get_property,
  adv_ndio_get_nth_property,
  adv_ndio_get_property_int32,
  adv_ndio_get_property_float64,
  adv_ndio_check_property_v,

  adv_ndio_read_octet,
  adv_ndio_read_string_length,
  adv_ndio_read_string,
  adv_ndio_read_int8,
  adv_ndio_read_int8v,
  adv_ndio_read_int16,
  adv_ndio_read_int16v,
  adv_ndio_read_int32,
  adv_ndio_read_int32v,
  adv_ndio_read_int64,
  adv_ndio_read_int64v,
  adv_ndio_read_float32,
  adv_ndio_read_float32v,
  adv_ndio_read_float64,
  adv_ndio_read_float64v,

  adv_ndio_set_property,
  adv_ndio_set_property_int32,
  adv_ndio_set_property_float64,
  adv_ndio_unset_nth_property,

  adv_ndio_write_octet,
  adv_ndio_write_string,
  adv_ndio_write_int8,
  adv_ndio_write_int8v,
  NULL, //adv_ndio_write_int16,
  NULL, //adv_ndio_write_int16v,
  adv_ndio_write_int32,
  adv_ndio_write_int32v,
  NULL, //adv_ndio_write_int64,
  NULL, //adv_ndio_write_int64v,
  NULL, //adv_ndio_write_float32,
  NULL, //adv_ndio_write_float32v,
  adv_ndio_write_float64,
  adv_ndio_write_float64v,
#endif
};

#endif /* HAVE_ORBIT */

/*------------------------------------------------------------*/

void
adv_dio_init(int* argc, char*** argv)
{
#ifdef HAVE_ORBIT
  adv_idio_init(argc, argv);
#endif
}

AdvDocument*
adv_dio_new_from_fdoc(AdvFDocument* fdoc)
{

  if (fdoc) 
    {
      AdvDocument* doc = a_new(AdvDocument);
      doc->doc = fdoc;
      doc->f = &fdoc_funcs;
      doc->close_original = true;
      doc->keep_alive = false;
      return doc;
    }
  else
    return NULL;
}

AdvDocument*
adv_dio_new_from_idoc(AdvIDocument* ndoc)
{
#ifdef HAVE_ORBIT
  if (ndoc)
    {
      AdvDocument* doc = a_new(AdvDocument);
      doc->doc = ndoc;
      doc->f = &idoc_funcs;
      doc->close_original = true;
      doc->keep_alive = false;
      return doc;
    }
  else
    return NULL;
#else
  return NULL;
#endif
}

AdvDocument*
adv_dio_open_by_property_v (AdvDocFile* dfile, void* prev, va_list va)
{
  void* doc;
  doc = dfile->f->open_by_property_v(dfile->dfile, prev, va);
  return dfile->f->convert(doc);
}

AdvDocument*
adv_dio_open_by_locator(const char* locator)
{
  if (strncasecmp(locator, "advdoc:", 7) == 0)
    locator += 7;

  if (strncasecmp(locator, "IOR:", 4) == 0)
    {
#if HAVE_ORBIT
      AdvIDocument* idoc = adv_idio_open_by_locator(locator);
      if (idoc)
	{
	  AdvDocument* doc = adv_dio_new_from_idoc(idoc);
	  doc->keep_alive = false;
	  return doc;
	}
      else
	{
	  return NULL;
	}
#else
      return NULL;
#endif
    }
  else 
    {
      AdvFDocument* fdoc = adv_fdio_open_by_locator(locator);
      if (fdoc)
	{
	  AdvDocument* doc = adv_dio_new_from_fdoc(fdoc);
	  doc->keep_alive = false;
	  return doc;
	}
    }
  return NULL;
}

void
adv_dio_close(AdvDocument* doc)
{
  if (!doc->keep_alive)
    {
      if (doc->close_original)
	(doc->f->close)(doc->doc);
      a_free(doc);
    }
}

/*------------------------------------------------------------
  adv_dio_document_make_documentid
  - make unique id.
  ------------------------------------------------------------*/

const char*
adv_dio_make_documentid(const char* str)
{
  static char id[1024];

  sprintf(id, "%X:%s:%X:%X", (unsigned int) random(), str, getpid(), 
                             (unsigned int) time(NULL));
  return id;
}

const char*
adv_dio_get_documentid(AdvDocument* doc)
{
  return (doc->f->get_documentid)(doc->doc);
}

adv_off_t
adv_dio_get_size(AdvDocument* doc)
{
  return (doc->f->get_size)(doc->doc);
}

const char*
adv_dio_get_property(AdvDocument* doc, const char* key)
{
  return (doc->f->get_property)(doc->doc, key);
}

const char*
adv_dio_get_locator(AdvDocument* doc)
{
  const char* p;
  p = (*(doc->f->get_locator))(doc->doc);
  return p;
}

/*--- property reading ---*/

bool
adv_dio_get_nth_property(AdvDocument* doc,
			 int n,
			 char* key, int keysize,
			 char* val, int valsize)
{
  return (doc->f->get_nth_property)(doc->doc, n, key, keysize, val, valsize);
}

bool
adv_dio_get_property_int32 (AdvDocument* doc,
			    const char* key,
			    int32* val)
{
  return (doc->f->get_property_int32)(doc->doc, key, val);
}

bool
adv_dio_get_property_float64 (AdvDocument* doc,
			      const char* key,
			      float64* val)
{
  return (doc->f->get_property_float64)(doc->doc, key, val);
}

bool
adv_dio_check_property(AdvDocument* doc,
		       ...)
{
  va_list va;
  bool ret;
  va_start(va, doc);
  ret = (doc->f->check_property_v)(doc->doc, va);
  va_end(va);
  return ret;
}

bool
adv_dio_check_property_v(AdvDocument* doc,
			 va_list va)
{
  return (doc->f->check_property_v)(doc->doc, va);
}

/*--- reading ---*/

int32
adv_dio_read_octet (AdvDocument* doc, adv_off_t offset, int32 len, octet* buf)
{
  return (doc->f->read_octet)(doc->doc, offset, len, buf);
}

int32
adv_dio_read_string_length (AdvDocument* doc, adv_off_t offset)
{
  return (doc->f->read_string_length)(doc->doc, offset);
}

int32
adv_dio_read_string (AdvDocument* doc, adv_off_t offset, char* buf)
{
  return (doc->f->read_string)(doc->doc, offset, buf);
}

int32
adv_dio_read_int8 (AdvDocument* doc, adv_off_t offset, int8* val)
{
  return (doc->f->read_int8)(doc->doc, offset, val);
}

int32
adv_dio_read_int8v (AdvDocument* doc, adv_off_t offset, int num, int8* val)
{
  return (doc->f->read_int8v)(doc->doc, offset, num, val);
}

int32
adv_dio_read_int16 (AdvDocument* doc, adv_off_t offset, int16* val)
{
  return (doc->f->read_int16)(doc->doc, offset, val);
}

int32
adv_dio_read_int16v (AdvDocument* doc, adv_off_t offset, int num, int16* val)
{
  return (doc->f->read_int16v)(doc->doc, offset, num, val);
}

int32
adv_dio_read_int32 (AdvDocument* doc, adv_off_t offset, int32* val)
{
  return (doc->f->read_int32)(doc->doc, offset, val);
}

int32
adv_dio_read_int32v (AdvDocument* doc, adv_off_t offset, int num, int32* val)
{
  return (doc->f->read_int32v)(doc->doc, offset, num, val);
}

#ifdef HAVE_ADVSYS_INT64

int32
adv_dio_read_int64 (AdvDocument* doc, adv_off_t offset, int64* val)
{
  return (doc->f->read_int64)(doc->doc, offset, val);
}

int32
adv_dio_read_int64v (AdvDocument* doc, adv_off_t offset, int num, int64* val)
{
  return (doc->f->read_int64v)(doc->doc, offset, num, val);
}

#else

int32
adv_dio_read_int64 (AdvDocument* doc, adv_off_t offset, int32* val)
{
  return (doc->f->read_int64)(doc->doc, offset, val);
}

int32
adv_dio_read_int64v (AdvDocument* doc, adv_off_t offset, int num, int32* val)
{
  return (doc->f->read_int64v)(doc->doc, offset, num, val);
}

#endif

int32
adv_dio_read_float32 (AdvDocument* doc, adv_off_t offset, float32* val)
{
  return (doc->f->read_float32)(doc->doc, offset, val);
}

int32
adv_dio_read_float32v (AdvDocument* doc, adv_off_t offset, int num, float32* val)
{
  return (doc->f->read_float32v)(doc->doc, offset, num, val);
}

int32
adv_dio_read_float64 (AdvDocument* doc, adv_off_t offset, float64* val)
{
  return (doc->f->read_float64)(doc->doc, offset, val);
}

int32
adv_dio_read_float64v (AdvDocument* doc, adv_off_t offset, int num, float64* val)
{
  return (doc->f->read_float64v)(doc->doc, offset, num, val);
}

/*--- property writing ---*/

void
adv_dio_set_property (AdvDocument* doc,
		      const char* key,
		      const char* val)
{ (doc->f->set_property)(doc->doc, key, val); }


void
adv_dio_set_property_int32 (AdvDocument* doc,
			    const char* key,
			    int32 val)
{ (doc->f->set_property_int32)(doc->doc, key, val); }

void
adv_dio_set_property_float64 (AdvDocument* doc,
			      const char* key,
			      float64 val)
{ (doc->f->set_property_float64)(doc->doc, key, val); }

void
adv_dio_unset_nth_property (AdvDocument* doc,
			    int n)
{ (doc->f->unset_nth_property)(doc->doc, n); }

/*--- writing ---*/

int32
adv_dio_write_octet (AdvDocument* doc, adv_off_t offset, int32 length, const octet* buf)
{ return (doc->f->write_octet)(doc->doc, offset, length, buf); }

int32
adv_dio_write_string (AdvDocument* doc, adv_off_t offset, const char* buf)
{ return (doc->f->write_string)(doc->doc, offset, buf); }

int32
adv_dio_write_int8 (AdvDocument* doc, adv_off_t offset, int8 val)
{ return (doc->f->write_int8)(doc->doc, offset, val); }

int32
adv_dio_write_int8v (AdvDocument* doc, adv_off_t offset, int num, const int8* val)
{ return (doc->f->write_int8v)(doc->doc, offset, num, val); }

int32
adv_dio_write_int16 (AdvDocument* doc, adv_off_t offset, int16 val)
{ return (doc->f->write_int16)(doc->doc, offset, val); }

int32
adv_dio_write_int16v (AdvDocument* doc, adv_off_t offset, int num, const int16* val)
{ return (doc->f->write_int16v)(doc->doc, offset, num, val); }

int32
adv_dio_write_int32 (AdvDocument* doc, adv_off_t offset, int32 val)
{ return (doc->f->write_int32)(doc->doc, offset, val); }

int32
adv_dio_write_int32v (AdvDocument* doc, adv_off_t offset, int num, const int32* val)
{ return (doc->f->write_int32v)(doc->doc, offset, num, val); }

#ifdef HAVE_ADVSYS_INT64
int32
adv_dio_write_int64 (AdvDocument* doc, adv_off_t offset, int64 val)
{ return (doc->f->write_int64)(doc->doc, offset, val); }

int32
adv_dio_write_int64v (AdvDocument* doc, adv_off_t offset, int num, const int64* val)
{ return (doc->f->write_int64v)(doc->doc, offset, num, val); }
#else
int32
adv_dio_write_int64 (AdvDocument* doc, adv_off_t offset, int32 val)
{ return (doc->f->write_int64)(doc->doc, offset, val); }

int32
adv_dio_write_int64v (AdvDocument* doc, adv_off_t offset, int num, const int32* val)
{ return (doc->f->write_int64v)(doc->doc, offset, num, val); }
#endif

int32
adv_dio_write_float32  (AdvDocument* doc, adv_off_t offset, float32 val)
{ return (doc->f->write_float32)(doc->doc, offset, val); }

int32
adv_dio_write_float32v (AdvDocument* doc, adv_off_t offset, int num, const float32* val)
{ return (doc->f->write_float32v)(doc->doc, offset, num, val); }

int32
adv_dio_write_float64  (AdvDocument* doc, adv_off_t offset, float64 val)
{ return (doc->f->write_float64)(doc->doc, offset, val); }

int32
adv_dio_write_float64v (AdvDocument* doc, adv_off_t offset, int num, const float64* val)
{
  int l;
  l = (doc->f->write_float64v)(doc->doc, offset, num, val);
  return l;
}

/*============================================================
  ADV Databox access
  ============================================================*/

typedef struct _DataboxSDoc
{
  AdvFDocument* doc;
  AdvFDocFile* dfile;
} DataboxSDoc;

struct _AdvDatabox
{
  AList* doc_list;				  /* list of AdvDocument*.
						     all docs in dfile_list are added to
						     this list temporary.
						     The `keep_alive' flag must be set
						     on every docs of this list. */
  ATree* doc_tree;				  /* B-Tree of AdvDocument.
						     Same as above, but use b-tree
						     to search speed up */

  AList* dfile_list;				  /* for storage management */
};

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

AdvDatabox*
adv_dbox_new()
{
  AdvDatabox* adb;
  adb = (AdvDatabox*)malloc(sizeof(AdvDatabox));
  adb->doc_list = NULL;
  adb->doc_tree = a_tree_new(cmp_did);
  adb->dfile_list = NULL;
  return adb;
}

bool
adv_dbox_add(AdvDatabox* adb, const char* locator)
{
  const char* p;
  bool done;
  done = false;

  if (strncasecmp(locator, "advdoc:", 7) == 0)
    locator += 7;

  if (strncasecmp(locator, "IOR:", 4) == 0)
    {
      AdvDocument* doc = adv_dio_open_by_locator(locator);
      if (doc)
	{					  /* network document */
	  doc->keep_alive = true;
	  adb->doc_list = a_list_prepend(adb->doc_list, doc);
	  adb->doc_tree = a_tree_insert(adb->doc_tree, doc, (void*)adv_dio_get_documentid(doc));
	  done = true;
	}
    }
  if (!done && ((p = strchr(locator, '?')) != NULL))
    {
      /* single file document */
      AdvDocument* doc = adv_dio_open_by_locator(locator);
      if (doc)
	{
	  doc->keep_alive = true;
	  adb->doc_list = a_list_prepend(adb->doc_list, doc);
	  adb->doc_tree = a_tree_insert(adb->doc_tree, doc, (void*)adv_dio_get_documentid(doc));
	  done = true;
	}
    }
  if (!done)
    {
      struct stat statbuf;
      if (stat(locator, &statbuf) == 0)
	{
	  if (S_ISDIR(statbuf.st_mode))
	    {
	      /* datadir */
	      DIR* dir;
	      struct dirent* dent;
	      dir = opendir(locator);
	      if ( (dent = readdir(dir)) != NULL )
		do
		  {
		    char path[_POSIX_PATH_MAX+1];
		    sprintf(path, "%s/%s", locator, dent->d_name);
		    if (stat(path, &statbuf) == 0 && S_ISREG(statbuf.st_mode))
		      adv_dbox_add(adb, path);
		  } while ( (dent = readdir(dir)) != NULL );
	      done = true;
	    }
	}
      if (!done)
	{
	  /* datafile */
	  AdvDocument* doc;
	  int i;
	  AdvDocFile* dfile = adv_dio_file_open(locator, "r");

#ifdef DEBUG
	  fprintf (stdout, "adv_dio_file_open called.(locator=[%s]\n)", locator);
#endif


	  if (dfile)
	    {
	      adb->dfile_list = a_list_prepend(adb->dfile_list, dfile);
	      i = 0;
	      if ( (doc = adv_dio_open_nth(dfile, i)) != NULL )
		{
		  do
		    {
		      i++;
		      doc->close_original = true;
		      doc->keep_alive = true;
		      adb->doc_list = a_list_prepend(adb->doc_list, doc);
		      adb->doc_tree = a_tree_insert(adb->doc_tree, doc, (void*)adv_dio_get_documentid(doc));

#ifdef DEBUG
		      fprintf (stdout, "[%d] doc[%s] append.\n", i, adv_dio_get_documentid(doc));
#endif

		    } while ( (doc = adv_dio_open_nth(dfile, i)) != NULL );
		}
	      done = true;
	    }
	}
    }
  return done;
}

void
adv_dbox_close(AdvDatabox* adb)
{
  AList* l;
  a_tree_destroy(adb->doc_tree);
  for (l = adb->doc_list; l != NULL; l = l->next)
    {
      AdvDocument* doc = (AdvDocument*)l->data;
      doc->keep_alive = false;
      adv_dio_close(doc);
    }
  a_list_destroy(adb->doc_list);
  for (l = adb->dfile_list; l != NULL; l = l->next)
    {
      AdvDocFile* dfile;
      dfile = (AdvDocFile*)l->data;
      adv_dio_file_close(dfile);
    }
  a_list_destroy(adb->dfile_list);
  free(adb);
}

AdvDocument*
adv_dbox_find_by_documentid(AdvDatabox* adb, const char* docid)
{
#if 0
  AList* l;
  for (l = adb->doc_list; l != NULL; l = l->next)
    {
      AdvDocument* doc = (AdvDocument*)l->data;
      const char* id = adv_dio_get_documentid(doc);
      if (id && strcmp(docid, id) == 0)
	return doc;
    }
#else
  AdvDocument* doc;
  doc = a_tree_lookup(adb->doc_tree, (void*)docid);
  if (doc)
    return doc;
#endif
  return NULL;
}

static
AdvDocument*
adv_dbox_find_by_property_v(AdvDatabox* adb, AdvDocument* prev, va_list va)
{
  AList* l;
  va_list va2;
  bool b = prev ? false : true;
  for (l = adb->doc_list; l != NULL; l = l->next)
    {
      AdvDocument* doc = (AdvDocument*)l->data;
      va_copy(va2, va);
      if (adv_dio_check_property_v(doc, va2))
	{
	  if (b)
	    return doc;
	  else if (doc == prev)
	    b = true;
	}
      va_end(va2);
    }
  return NULL;
}

AdvDocument*
adv_dbox_find_by_property(AdvDatabox* adb, AdvDocument* prev, ...)
{
  va_list va;
  AdvDocument* doc;
  va_start(va, prev);
  doc = adv_dbox_find_by_property_v(adb, prev, va);
  va_end(va);
  return doc;
}

int
adv_dbox_count_by_property(AdvDatabox* adb, ...)
{
  AList* l;
  va_list va;
  int count;

  count = 0;
  for (l = adb->doc_list; l != NULL; l = l->next)
    {
      AdvDocument* doc = (AdvDocument*)l->data;
      va_start(va, adb);
      if (adv_dio_check_property_v(doc, va))
	count++;
      va_end(va);
    }
  return count;
}

AdvDocument*
adv_dbox_open_nth(AdvDatabox* adb, int n)
{
  AList* l;
  for (l = adb->doc_list; l != NULL; l = l->next, n--)
    {
      if (n == 0)
        return (AdvDocument*)l->data;
    }
  return NULL;
}

/*------------------------------------------------------------
  format operation
  ------------------------------------------------------------*/

int
adv_format_get_size(const char* format)
{
  int i;
  int size = 0;
  i = 0;
  while (format[i] != '\0')
    {
      switch (format[i])
	{
	case 'i':
	case 'I':
	case 'f':
	case 'F':
	  break;
	default:
	  return -1;				  /* error */
	}
      i++;
      switch (format[i])
	{
	case '1':
	  size += 1;
	  break;
	case '2':
	  size += 2;
	  break;
	case '4':
	  size += 4;
	  break;
	case '8':
	  size += 8;
	  break;
	default:
	  return -1;				  /* error */
	}
      i++;
    }
  return size;
}

bool
adv_format_pack_v(octet* buf, const char* format, va_list va)
{
  int i;
  char atom_type;
  int atom_size;
  i = 0;
  while (format[i] != '\0')
    {
      switch (format[i])
	{
	case 'i':
	case 'I':
	  atom_type = 'i';
	  break;
	case 'f':
	case 'F':
	  atom_type = 'f';
	  break;
	default:
	  return false;				  /* error */
	}
      i++;
      switch (format[i])
	{
	case '1':
	  atom_size = 1;
	  if (atom_type == 'f')
	    return false;
	  break;
	case '2':
	  atom_size = 2;
	  if (atom_type == 'f')
	    return false;
	  break;
	case '4':
	  atom_size = 4;
	  break;
	case '8':
	  atom_size = 8;
	  break;
	default:
	  return -1;				  /* error */
	}
      i++;
      if (atom_size == 1)
	{
	  int8 v = va_arg(va, int);
	  memcpy(buf, &v, sizeof(int8));
	}
      else if (atom_size == 2)
	{
	  int16 v = va_arg(va, int);
	  memcpy(buf, &v, sizeof(int16));
#ifdef WORDS_BIGENDIAN
	  swap_octet(buf[0], buf[1]);
#endif
	}
      else if (atom_size == 4)
	{
	  if (atom_type == 'f')
	    {
	      float32 v = va_arg(va, double);
	      memcpy(buf, &v, sizeof(float32));
	    }
	  else
	    {
	      int32 v = va_arg(va, int);
	      memcpy(buf, &v, sizeof(int32));
	    }
#ifdef WORDS_BIGENDIAN
	  swap_octet(buf[0], buf[3]);
	  swap_octet(buf[1], buf[2]);
#endif
	}
      else if (atom_size == 8)
	{
	  if (atom_type == 'f')
	    {
	      float64 v = va_arg(va, double);
	      memcpy(buf, &v, sizeof(float64));
	    }
	  else
	    {
#ifdef HAVE_ADVSYS_INT64
	      int64 v = va_arg(va, int);
	      memcpy(buf, &v, sizeof(int64));
#endif
	    }
#ifdef WORDS_BIGENDIAN
	  swap_octet(buf[0], buf[7]);
	  swap_octet(buf[1], buf[6]);
	  swap_octet(buf[2], buf[5]);
	  swap_octet(buf[3], buf[4]);
#endif
	}
      buf += atom_size;
    }
  return true;
}

bool
adv_format_pack(octet* buf, const char* format, ...)
{
  bool b;
  va_list va;
  va_start(va, format);
  b = adv_format_pack_v(buf, format, va);
  va_end(va);
  return b;
}

bool
adv_format_unpack(octet* buf, const char* format, ...)
{
  int i;
  va_list va;
  char atom_type;
  int atom_size;
  octet* b;
  va_start(va, format);
  i = 0;
  while (format[i] != '\0')
    {
      switch (format[i])
	{
	case 'i':
	case 'I':
	  atom_type = 'i';
	  break;
	case 'f':
	case 'F':
	  atom_type = 'f';
	  break;
	default:
	  return false;				  /* error */
	}
      i++;
      switch (format[i])
	{
	case '1':
	  atom_size = 1;
	  if (atom_type == 'f')
	    return false;
	  break;
	case '2':
	  atom_size = 2;
	  if (atom_type == 'f')
	    return false;
	  break;
	case '4':
	  atom_size = 4;
	  break;
	case '8':
	  atom_size = 8;
	  break;
	default:
	  return -1;				  /* error */
	}
      i++;
      if (atom_size == 1)
	{
	  int8* v = va_arg(va, int8*);
	  memcpy(v, buf, sizeof(int8));
	}
      else if (atom_size == 2)
	{
	  int16* v = va_arg(va, int16*);
	  memcpy(v, buf, sizeof(int16));
#ifdef WORDS_BIGENDIAN
	  b = (octet*)v;
	  swap_octet(b[0], b[1]);
#endif
	}
      else if (atom_size == 4)
	{
	  if (atom_type == 'f')
	    {
	      float32* v = va_arg(va, float32*);
	      memcpy(v, buf, sizeof(float32));
	      b = (octet*)v;
	    }
	  else
	    {
	      int32* v = va_arg(va, int32*);
	      memcpy(v, buf, sizeof(int32));
	      b = (octet*)v;
	    }
#ifdef WORDS_BIGENDIAN
	  swap_octet(b[0], b[3]);
	  swap_octet(b[1], b[2]);
#endif
	}
      else if (atom_size == 8)
	{
	  if (atom_type == 'f')
	    {
	      float64* v = va_arg(va, float64*);
	      memcpy(v, buf, sizeof(float64));
	      b = (octet*)v;
	    }
	  else
	    {
#ifdef HAVE_ADVSYS_INT64
	      int64* v = va_arg(va, int64*);
	      memcpy(v, buf, sizeof(int64));
	      b = (octet*)v;
#endif
	    }
#ifdef WORDS_BIGENDIAN
	  swap_octet(b[0], b[7]);
	  swap_octet(b[1], b[6]);
	  swap_octet(b[2], b[5]);
	  swap_octet(b[3], b[4]);
#endif
	}
      buf += atom_size;
    }
  va_end(va);
  return true;
}

void
adv_dio_copy_to_file (AdvDocFile* dfile, AdvDocument* src)
{
  char key[1024];
  char val[1024];
  int i;
  adv_off_t left, off;
  octet buf[4096];
  const char* did = adv_dio_get_documentid(src);
  AdvDocument* dest = adv_dio_create(dfile, did);
  i = 0;
  while (adv_dio_get_nth_property(src, i++, key, sizeof(key), val, sizeof(val)))
    adv_dio_set_property(dest, key, val);
  left = adv_dio_get_size(src);
  off = 0;
  while (left > 0)
    { 
      adv_off_t read = left < sizeof(buf) ? left : sizeof(buf);
      adv_dio_read_octet(src, off, read, buf);
      adv_dio_write_octet(dest, off, read, buf);
      left -= read;
      off += read;
    }
  adv_dio_close(dest);
}
