/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#ifndef _AdvFDocument_h_included_
#define _AdvFDocument_h_included_

#include <Adv/AdvTypes.h>
#include <Adv/AdvFileIO.h>
#include <fcntl.h>
#include <stdarg.h>

#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END   };
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

EXTERN_C_BEGIN

typedef struct _AdvFDocFile  AdvFDocFile;
typedef struct _AdvFDocument AdvFDocument;

/*
  in case type of retval is bool,
  true: success, no error, ok
  false: fail, ng, error
*/  

/*------------------------------------------------------------
  document I/O
  ------------------------------------------------------------*/ 

/*----- document file I/O -----*/

/* mode : 'r' for read(default), 'c' for create, 'a' for append */
extern AdvFDocFile* adv_fdio_file_open           (const char* filename, const char* mode);
extern void         adv_fdio_file_close          (AdvFDocFile* dfile);
extern const char*  adv_fdio_file_get_locator    (AdvFDocFile* dfile);

/*----- open, close -----*/

extern AdvFDocument* adv_fdio_create             (AdvFDocFile* dfile, const char* did);

extern AdvFDocument* adv_fdio_open_by_documentid (AdvFDocFile* dfile, const char* documentid);
extern AdvFDocument* adv_fdio_open_nth           (AdvFDocFile* dfile, int n);
extern AdvFDocument* adv_fdio_open_by_property   (AdvFDocFile* dfile, void* prev, ...);
extern AdvFDocument* adv_fdio_open_by_property_v (AdvFDocFile* dfile, void* prev, va_list va);
extern AdvFDocument* adv_fdio_open_by_locator    (const char* locator);

extern void          adv_fdio_close              (AdvFDocument* doc);

/*----- status -----*/

extern const char*  adv_fdio_get_documentid      (AdvFDocument* doc);
extern const char*  adv_fdio_get_locator         (AdvFDocument* doc);
extern adv_off_t    adv_fdio_get_size            (AdvFDocument* doc);

/*----- property operation -----*/

extern void         adv_fdio_set_property         (AdvFDocument* doc,
						   const char* key,
						   const char* val);
extern void         adv_fdio_set_property_int32   (AdvFDocument* doc,
						   const char* key,
						   int32 val);
extern void         adv_fdio_set_property_float64 (AdvFDocument* doc,
						   const char* key,
						   float64 val);
extern void         adv_fdio_unset_nth_property   (AdvFDocument* doc,
						   int n);
extern const char*  adv_fdio_get_property         (AdvFDocument* doc,
						   const char* key);
extern bool         adv_fdio_get_nth_property     (AdvFDocument* doc,
						   int n,
						   char* key, int keysize,
						   char* val, int valsize);
extern bool         adv_fdio_get_property_int32   (AdvFDocument* doc,
						   const char* key,
						   int32* val);
extern bool         adv_fdio_get_property_float64 (AdvFDocument* doc,
						   const char* key,
						   float64* val);
extern bool         adv_fdio_check_property       (AdvFDocument* doc,
						   ...);
extern bool         adv_fdio_check_property_v     (AdvFDocument* doc,
						   va_list va);

/*--- read ---*/
/*
  v(like read_int32v) means vector
*/

extern int32 adv_fdio_read_octet    (AdvFDocument* doc, adv_off_t offset, int32 len, octet* buf);
extern int32 adv_fdio_read_string_length (AdvFDocument* doc, adv_off_t offset);
extern int32 adv_fdio_read_string   (AdvFDocument* doc, adv_off_t offset, char* val);
extern int32 adv_fdio_read_int8     (AdvFDocument* doc, adv_off_t offset, int8* val);
extern int32 adv_fdio_read_int8v    (AdvFDocument* doc, adv_off_t offset, int num, int8* val);
extern int32 adv_fdio_read_int16    (AdvFDocument* doc, adv_off_t offset, int16* val);
extern int32 adv_fdio_read_int16v   (AdvFDocument* doc, adv_off_t offset, int num, int16* val);
extern int32 adv_fdio_read_int32    (AdvFDocument* doc, adv_off_t offset, int32* val);
extern int32 adv_fdio_read_int32v   (AdvFDocument* doc, adv_off_t offset, int num, int32* val);
#ifdef HAVE_ADVSYS_INT64
extern int32 adv_fdio_read_int64    (AdvFDocument* doc, adv_off_t offset, int64* val);
extern int32 adv_fdio_read_int64v   (AdvFDocument* doc, adv_off_t offset, int num, int64* val);
#else
extern int32 adv_fdio_read_int64    (AdvFDocument* doc, adv_off_t offset, int32* val);
extern int32 adv_fdio_read_int64v   (AdvFDocument* doc, adv_off_t offset, int num, int32* val);
#endif
extern int32 adv_fdio_read_float32  (AdvFDocument* doc, adv_off_t offset, float32* val);
extern int32 adv_fdio_read_float32v (AdvFDocument* doc, adv_off_t offset, int num, float32* val);
extern int32 adv_fdio_read_float64  (AdvFDocument* doc, adv_off_t offset, float64* val);
extern int32 adv_fdio_read_float64v (AdvFDocument* doc, adv_off_t offset, int num, float64* val);

/*--- write ---*/

extern int32 adv_fdio_write_octet    (AdvFDocument* doc, adv_off_t offset, int32 length, const octet* buf);
extern int32 adv_fdio_write_string   (AdvFDocument* doc, adv_off_t offset, const char* val);
extern int32 adv_fdio_write_int8     (AdvFDocument* doc, adv_off_t offset, int8 val);
extern int32 adv_fdio_write_int8v    (AdvFDocument* doc, adv_off_t offset, int num, const int8* val);
extern int32 adv_fdio_write_int16    (AdvFDocument* doc, adv_off_t offset, int16 val);
extern int32 adv_fdio_write_int16v   (AdvFDocument* doc, adv_off_t offset, int num, const int16* val);
extern int32 adv_fdio_write_int32    (AdvFDocument* doc, adv_off_t offset, int32 val);
extern int32 adv_fdio_write_int32v   (AdvFDocument* doc, adv_off_t offset, int num, const int32* val);
#ifdef HAVE_ADVSYS_INT64
extern int32 adv_fdio_write_int64    (AdvFDocument* doc, adv_off_t offset, int64 val);
extern int32 adv_fdio_write_int64v   (AdvFDocument* doc, adv_off_t offset, int num, const int64* val);
#else
extern int32 adv_fdio_write_int64    (AdvFDocument* doc, adv_off_t offset, int32 val);
extern int32 adv_fdio_write_int64v   (AdvFDocument* doc, adv_off_t offset, int num, const int32* val);
#endif
extern int32 adv_fdio_write_float32  (AdvFDocument* doc, adv_off_t offset, float32 val);
extern int32 adv_fdio_write_float32v (AdvFDocument* doc, adv_off_t offset, int num, const float32* val);
extern int32 adv_fdio_write_float64  (AdvFDocument* doc, adv_off_t offset, float64 val);
extern int32 adv_fdio_write_float64v (AdvFDocument* doc, adv_off_t offset, int num, const float64* val);

EXTERN_C_END

#endif
