/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#ifndef _adv_document_private_included_
#define _adv_document_private_included_

typedef AdvDocument* (*DFILE_CONVERT_FUNC)            (void* doc);
typedef void         (*DFILE_FILE_OPEN_FUNC)          (const char* loc, const char* mode);
typedef void         (*DFILE_FILE_CLOSE_FUNC)         (void* dfile);
typedef void*        (*DFILE_CREATE_FUNC)             (void* dfile, const char* did);
typedef void*        (*DFILE_OPEN_BY_DOCUMENTID_FUNC) (void* dfile, const char* did);
typedef void*        (*DFILE_OPEN_NTH_FUNC)           (void* dfile, int n);
typedef void*        (*DFILE_OPEN_BY_PROPERTY_V_FUNC) (void* dfile, void* prev, va_list va);
typedef const char*  (*DFILE_GET_LOCATOR_FUNC)        (void* dfile);

typedef void        (*DOC_CLOSE_FUNC)                (void* doc);

typedef const char* (*DOC_GET_DOCUMENTID_FUNC)       (void* doc);
typedef adv_off_t   (*DOC_GET_SIZE_FUNC)             (void* doc);
typedef const char* (*DOC_GET_LOCATOR_FUNC)          (void* doc);

typedef const char* (*DOC_GET_PROPERTY_FUNC)         (void* doc, const char* key);
typedef bool        (*DOC_GET_NTH_PROPERTY_FUNC)     (void* doc, int n,
						      char* key, int keysize,
						      char* val, int valsize);
typedef bool        (*DOC_GET_PROPERTY_INT32_FUNC)   (void* doc, const char* key, int32* val);
typedef bool        (*DOC_GET_PROPERTY_FLOAT64_FUNC) (void* doc, const char* key, float64* val);
typedef bool        (*DOC_CHECK_PROPERTY_V_FUNC)     (void* doc, va_list va);

typedef int32       (*DOC_READ_OCTET_FUNC)           (void* doc, adv_off_t offset, int32 len, octet* buf);
typedef int32       (*DOC_READ_STRING_LENGTH_FUNC)   (void* doc, adv_off_t offset);
typedef int32       (*DOC_READ_STRING_FUNC)          (void* doc, adv_off_t offset, char* buf);
typedef int32       (*DOC_READ_INT8_FUNC)            (void* doc, adv_off_t offset, int8* val);
typedef int32       (*DOC_READ_INT8V_FUNC)           (void* doc, adv_off_t offset, int num, int8* val);
typedef int32       (*DOC_READ_INT16_FUNC)           (void* doc, adv_off_t offset, int16* val);
typedef int32       (*DOC_READ_INT16V_FUNC)          (void* doc, adv_off_t offset, int num, int16* val);
typedef int32       (*DOC_READ_INT32_FUNC)           (void* doc, adv_off_t offset, int32* val);
typedef int32       (*DOC_READ_INT32V_FUNC)          (void* doc, adv_off_t offset, int num, int32* val);
#ifdef HAVE_ADVSYS_INT64
typedef int32       (*DOC_READ_INT64_FUNC)           (void* doc, adv_off_t offset, int64* val);
typedef int32       (*DOC_READ_INT64V_FUNC)          (void* doc, adv_off_t offset, int num, int64* val);
#else
typedef int32       (*DOC_READ_INT64_FUNC)           (void* doc, adv_off_t offset, int32* val);
typedef int32       (*DOC_READ_INT64V_FUNC)          (void* doc, adv_off_t offset, int num, int32* val);
#endif
typedef int32       (*DOC_READ_FLOAT32_FUNC)         (void* doc, adv_off_t offset, float32* val);
typedef int32       (*DOC_READ_FLOAT32V_FUNC)        (void* doc, adv_off_t offset, int num, float32* val);
typedef int32       (*DOC_READ_FLOAT64_FUNC)         (void* doc, adv_off_t offset, float64* val);
typedef int32       (*DOC_READ_FLOAT64V_FUNC)        (void* doc, adv_off_t offset, int num, float64* val);

typedef void        (*DOC_SET_PROPERTY_FUNC)         (void* doc, const char* key, const char* val);
typedef void        (*DOC_SET_PROPERTY_INT32_FUNC)   (void* doc, const char* key, int32 val);
typedef void        (*DOC_SET_PROPERTY_FLOAT64_FUNC) (void* doc, const char* key, float64 val);
typedef void        (*DOC_UNSET_NTH_PROPERTY_FUNC)   (void* doc, int n);

typedef int32       (*DOC_WRITE_OCTET_FUNC)         (void* doc, adv_off_t offset, int32 length, const octet* buf);
typedef int32       (*DOC_WRITE_STRING_FUNC)        (void* doc, adv_off_t offset, const char* buf);
typedef int32       (*DOC_WRITE_INT8_FUNC)          (void* doc, adv_off_t offset, int8 val);
typedef int32       (*DOC_WRITE_INT8V_FUNC)         (void* doc, adv_off_t offset, int num, const int8* val);
typedef int32       (*DOC_WRITE_INT16_FUNC)         (void* doc, adv_off_t offset, int16 val);
typedef int32       (*DOC_WRITE_INT16V_FUNC)        (void* doc, adv_off_t offset, int num, const int16* val);
typedef int32       (*DOC_WRITE_INT32_FUNC)         (void* doc, adv_off_t offset, int32 val);
typedef int32       (*DOC_WRITE_INT32V_FUNC)        (void* doc, adv_off_t offset, int num, const int32* val);
#ifdef HAVE_ADVSYS_INT64
typedef int32       (*DOC_WRITE_INT64_FUNC)         (void* doc, adv_off_t offset, int64 val);
typedef int32       (*DOC_WRITE_INT64V_FUNC)        (void* doc, adv_off_t offset, int num, const int64* val);
#else
typedef int32       (*DOC_WRITE_INT64_FUNC)         (void* doc, adv_off_t offset, int32 val);
typedef int32       (*DOC_WRITE_INT64V_FUNC)        (void* doc, adv_off_t offset, int num, const int32* val);
#endif
typedef int32       (*DOC_WRITE_FLOAT32_FUNC)       (void* doc, adv_off_t offset, float32 val);
typedef int32       (*DOC_WRITE_FLOAT32V_FUNC)      (void* doc, adv_off_t offset, int num, const float32* val);
typedef int32       (*DOC_WRITE_FLOAT64_FUNC)       (void* doc, adv_off_t offset, float64 val);
typedef int32       (*DOC_WRITE_FLOAT64V_FUNC)      (void* doc, adv_off_t offset, int num, const float64* val);


typedef struct _AdvDocFileFuncs
{
  DFILE_CONVERT_FUNC            convert;
  DFILE_FILE_OPEN_FUNC          file_open;
  DFILE_FILE_CLOSE_FUNC         file_close;
  DFILE_CREATE_FUNC             create;
  DFILE_OPEN_BY_DOCUMENTID_FUNC open_by_documentid;
  DFILE_OPEN_NTH_FUNC           open_nth;
  DFILE_OPEN_BY_PROPERTY_V_FUNC open_by_property_v;
  DFILE_GET_LOCATOR_FUNC        get_locator;
} AdvDocFileFuncs;

struct _AdvDocFile
{
  void* dfile;
  AdvDocFileFuncs* f;
};

typedef struct _AdvDocumentFuncs
{
  /*--- close ---*/
  DOC_CLOSE_FUNC close;

  /*--- stat --*/
  DOC_GET_DOCUMENTID_FUNC get_documentid;
  DOC_GET_SIZE_FUNC       get_size;
  DOC_GET_LOCATOR_FUNC    get_locator;

  /*--- property read ---*/
  DOC_GET_PROPERTY_FUNC         get_property;
  DOC_GET_NTH_PROPERTY_FUNC     get_nth_property;
  DOC_GET_PROPERTY_INT32_FUNC   get_property_int32;
  DOC_GET_PROPERTY_FLOAT64_FUNC get_property_float64;
  DOC_CHECK_PROPERTY_V_FUNC     check_property_v;

  /*--- read ---*/
  DOC_READ_OCTET_FUNC          read_octet;
  DOC_READ_STRING_LENGTH_FUNC  read_string_length;
  DOC_READ_STRING_FUNC         read_string;
  int32 (*read_int8)           (void* doc, adv_off_t offset, int8* val);
  int32 (*read_int8v)          (void* doc, adv_off_t offset, int num, int8* val);
  int32 (*read_int16)          (void* doc, adv_off_t offset, int16* val);
  int32 (*read_int16v)         (void* doc, adv_off_t offset, int num, int16* val);
  int32 (*read_int32)          (void* doc, adv_off_t offset, int32* val);
  int32 (*read_int32v)         (void* doc, adv_off_t offset, int num, int32* val);
#ifdef HAVE_ADVSYS_INT64
  int32 (*read_int64)          (void* doc, adv_off_t offset, int64* val);
  int32 (*read_int64v)         (void* doc, adv_off_t offset, int num, int64* val);
#else
  int32 (*read_int64)          (void* doc, adv_off_t offset, int32* val);
  int32 (*read_int64v)         (void* doc, adv_off_t offset, int num, int32* val);
#endif
  int32 (*read_float32)        (void* doc, adv_off_t offset, float32* val);
  int32 (*read_float32v)       (void* doc, adv_off_t offset, int num, float32* val);
  int32 (*read_float64)        (void* doc, adv_off_t offset, float64* val);
  int32 (*read_float64v)       (void* doc, adv_off_t offset, int num, float64* val);

  /*--- property write ---*/
  void (*set_property)         (void* doc, const char* key, const char* val);
  void (*set_property_int32)   (void* doc, const char* key, int32 val);
  void (*set_property_float64) (void* doc, const char* key, float64 val);
  void (*unset_nth_property)   (void* doc, int n);

  /*--- write ---*/
  int32 (*write_octet)         (void* doc, adv_off_t offset, int32 length, const octet* buf);
  int32 (*write_string)        (void* doc, adv_off_t offset, const char* buf);
  int32 (*write_int8)          (void* doc, adv_off_t offset, int8 val);
  int32 (*write_int8v)         (void* doc, adv_off_t offset, int num, const int8* val);
  int32 (*write_int16)         (void* doc, adv_off_t offset, int16 val);
  int32 (*write_int16v)        (void* doc, adv_off_t offset, int num, const int16* val);
  int32 (*write_int32)         (void* doc, adv_off_t offset, int32 val);
  int32 (*write_int32v)        (void* doc, adv_off_t offset, int num, const int32* val);
#ifdef HAVE_ADVSYS_INT64
  int32 (*write_int64)         (void* doc, adv_off_t offset, int64 val);
  int32 (*write_int64v)        (void* doc, adv_off_t offset, int num, const int64* val);
#else
  int32 (*write_int64)         (void* doc, adv_off_t offset, int32 val);
  int32 (*write_int64v)        (void* doc, adv_off_t offset, int num, const int32* val);
#endif
  int32 (*write_float32)       (void* doc, adv_off_t offset, float32 val);
  int32 (*write_float32v)      (void* doc, adv_off_t offset, int num, const float32* val);
  int32 (*write_float64)       (void* doc, adv_off_t offset, float64 val);
  int32 (*write_float64v)      (void* doc, adv_off_t offset, int num, const float64* val);
} AdvDocumentFuncs;

struct _AdvDocument
{
  void* doc;
  bool close_original;
  bool keep_alive;				  /* don't close on adv_dio_close.
						     this is used by databox. */
  AdvDocumentFuncs* f;
};

#endif /* _adv_document_private_included_ */

