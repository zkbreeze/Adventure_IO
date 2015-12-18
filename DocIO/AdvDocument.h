/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#ifndef _AdvDocument_h_included_
#define _AdvDocument_h_included_

#include "AdvFDocument.h"
#include "AdvIDocument.h"
#include "AdvMDocument.h"

#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END   };
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

EXTERN_C_BEGIN

typedef struct _AdvDocFile  AdvDocFile;
typedef struct _AdvDocument AdvDocument;
typedef struct _AdvDatabox  AdvDatabox;

/*------------------------------------------------------------
  Document
  ------------------------------------------------------------*/ 

/*----- init -----*/

extern void adv_dio_init(int* argc, char*** argv);

/*----- container -----*/

/* mode : 'r' for read(default), 'c' for create, 'a' for append */
extern AdvDocFile*  adv_dio_file_open           (const char* locator, const char* mode);

extern void         adv_dio_file_close          (AdvDocFile* dfile);
extern const char*  adv_dio_file_get_locator    (AdvDocFile* dfile);

extern AdvDocFile*  adv_dio_file_new_from_idfile (AdvIDocFile* idfile);

/*----- open, close -----*/

/* convert */
extern AdvDocument* adv_dio_new_from_fdoc       (AdvFDocument* fdoc);
extern AdvDocument* adv_dio_new_from_idoc       (AdvIDocument* ndoc);

/* for write */
extern AdvDocument* adv_dio_create              (AdvDocFile* dfile, const char* did);

/* for read */
extern AdvDocument* adv_dio_open_by_documentid  (AdvDocFile* dfile, const char* did);
extern AdvDocument* adv_dio_open_nth            (AdvDocFile* dfile, int n);
extern AdvDocument* adv_dio_open_by_property    (AdvDocFile* dfile, void* prev, ...);
extern AdvDocument* adv_dio_open_by_property_v  (AdvDocFile* dfile, void* prev, va_list va);
extern AdvDocument* adv_dio_open_by_locator     (const char* locator);

extern void adv_dio_close                       (AdvDocument* doc);

/*----- status -----*/

extern const char* adv_dio_make_documentid      (const char* str);
extern const char* adv_dio_get_documentid       (AdvDocument* doc);
extern adv_off_t   adv_dio_get_size             (AdvDocument* doc);
extern const char* adv_dio_get_locator          (AdvDocument* doc);

/*----- property read -----*/

extern void adv_dio_set_property                (AdvDocument* doc,
						 const char* key,
						 const char* val);
extern void adv_dio_set_property_int32          (AdvDocument* doc,
						 const char* key,
						 int32 val);
extern void adv_dio_set_property_float64        (AdvDocument* doc,
						 const char* key,
						 float64 val);
extern void adv_dio_unset_nth_property          (AdvDocument* doc,
						 int n);

extern const char* adv_dio_get_property         (AdvDocument* doc, const char* key);

extern bool        adv_dio_get_nth_property     (AdvDocument* doc,
						 int n,
						 char* key, int keysize,
						 char* val, int valsize);
extern bool        adv_dio_get_property_int32   (AdvDocument* doc,
						 const char* key,
						 int32* val);
extern bool        adv_dio_get_property_float64 (AdvDocument* doc,
						 const char* key,
						 float64* val);
extern bool        adv_dio_check_property       (AdvDocument* doc,
						 ...);
extern bool        adv_dio_check_property_v     (AdvDocument* doc,
						 va_list va);

/*----- read -----*/

extern int32 adv_dio_read_octet    (AdvDocument* doc, adv_off_t offset, int32 len, octet* buf);
extern int32 adv_dio_read_string_length (AdvDocument* doc, adv_off_t offset);
extern int32 adv_dio_read_string   (AdvDocument* doc, adv_off_t offset, char* buf);
extern int32 adv_dio_read_int8     (AdvDocument* doc, adv_off_t offset, int8* val);
extern int32 adv_dio_read_int8v    (AdvDocument* doc, adv_off_t offset, int num, int8* val);
extern int32 adv_dio_read_int16    (AdvDocument* doc, adv_off_t offset, int16* val);
extern int32 adv_dio_read_int16v   (AdvDocument* doc, adv_off_t offset, int num, int16* val);
extern int32 adv_dio_read_int32    (AdvDocument* doc, adv_off_t offset, int32* val);
extern int32 adv_dio_read_int32v   (AdvDocument* doc, adv_off_t offset, int num, int32* val);
#ifdef HAVE_ADVSYS_INT64
extern int32 adv_dio_read_int64    (AdvDocument* doc, adv_off_t offset, int64* val);
extern int32 adv_dio_read_int64v   (AdvDocument* doc, adv_off_t offset, int num, int64* val);
#else
extern int32 adv_dio_read_int64    (AdvDocument* doc, adv_off_t offset, int32* val);
extern int32 adv_dio_read_int64v   (AdvDocument* doc, adv_off_t offset, int num, int32* val);
#endif
extern int32 adv_dio_read_float32  (AdvDocument* doc, adv_off_t offset, float32* val);
extern int32 adv_dio_read_float32v (AdvDocument* doc, adv_off_t offset, int num, float32* val);
extern int32 adv_dio_read_float64  (AdvDocument* doc, adv_off_t offset, float64* val);
extern int32 adv_dio_read_float64v (AdvDocument* doc, adv_off_t offset, int num, float64* val);

/*----- write -----*/

extern int32 adv_dio_write_octet    (AdvDocument* doc, adv_off_t offset, int32 length, const octet* buf);
extern int32 adv_dio_write_string   (AdvDocument* doc, adv_off_t offset, const char* buf);
extern int32 adv_dio_write_int8     (AdvDocument* doc, adv_off_t offset, int8 val);
extern int32 adv_dio_write_int8v    (AdvDocument* doc, adv_off_t offset, int num, const int8* val);
extern int32 adv_dio_write_int16    (AdvDocument* doc, adv_off_t offset, int16 val);
extern int32 adv_dio_write_int16v   (AdvDocument* doc, adv_off_t offset, int num, const int16* val);
extern int32 adv_dio_write_int32    (AdvDocument* doc, adv_off_t offset, int32 val);
extern int32 adv_dio_write_int32v   (AdvDocument* doc, adv_off_t offset, int num, const int32* val);
#ifdef HAVE_ADVSYS_INT64
extern int32 adv_dio_write_int64    (AdvDocument* doc, adv_off_t offset, int64 val);
extern int32 adv_dio_write_int64v   (AdvDocument* doc, adv_off_t offset, int num, const int64* val);
#else
extern int32 adv_dio_write_int64    (AdvDocument* doc, adv_off_t offset, int32 val);
extern int32 adv_dio_write_int64v   (AdvDocument* doc, adv_off_t offset, int num, const int32* val);
#endif
extern int32 adv_dio_write_float32  (AdvDocument* doc, adv_off_t offset, float32 val);
extern int32 adv_dio_write_float32v (AdvDocument* doc, adv_off_t offset, int num, const float32* val);
extern int32 adv_dio_write_float64  (AdvDocument* doc, adv_off_t offset, float64 val);
extern int32 adv_dio_write_float64v (AdvDocument* doc, adv_off_t offset, int num, const float64* val);

/*------------------------------------------------------------
  special
  ------------------------------------------------------------*/ 

extern void adv_dio_copy_to_file (AdvDocFile* dfile, AdvDocument* doc);

/*------------------------------------------------------------
  databox I/O
  ------------------------------------------------------------*/ 

extern AdvDatabox*  adv_dbox_new(void);
extern bool         adv_dbox_add(AdvDatabox* adb, const char* locator);
extern bool         adv_dbox_add_document(AdvDatabox* adb, AdvDocument* doc);
extern bool         adv_dbox_add_docfile(AdvDatabox* adb, AdvDocFile* dfile);
extern void         adv_dbox_close(AdvDatabox* adb);
extern AdvDocument* adv_dbox_find_by_documentid(AdvDatabox* adb,
						const char* docid);
extern AdvDocument* adv_dbox_find_by_property(AdvDatabox* adb, AdvDocument* prev, ...);
extern int          adv_dbox_count_by_property(AdvDatabox* adb, ...);
extern AdvDocument* adv_dbox_open_nth(AdvDatabox* adb, int n);

/*------------------------------------------------------------
  format operation
  ------------------------------------------------------------*/

/* retval is bytelength of data represented by the format string. -1 means invalid format string*/
extern int  adv_format_get_size(const char* format);

extern bool adv_format_pack(octet* buf, const char* format, ...);
extern bool adv_format_pack_v(octet* buf, const char* format, va_list va);
extern bool adv_format_unpack(octet* buf, const char* format, ...);

EXTERN_C_END

#endif
