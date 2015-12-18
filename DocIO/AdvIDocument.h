/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#ifndef _AdvIDocument_h_included_
#define _AdvIDocument_h_included_

#include <Adv/AdvFileIO.h>

typedef struct _AdvIDocument AdvIDocument;
typedef struct _AdvIDocFile AdvIDocFile;

#ifdef __cplusplus
extern "C" {
#endif

extern void          adv_idio_init               (int* argc, char*** argv);

extern AdvIDocFile*  adv_idio_file_open          (const char* loc, const char* mode);
extern void          adv_idio_file_close         (AdvIDocFile* dfile);
extern void          adv_idio_file_clear         (AdvIDocFile* dfile);
extern const char*   adv_idio_file_get_locator   (AdvIDocFile* dfile);

/*----- open & close -----*/

extern AdvIDocument* adv_idio_create             (AdvIDocFile* dfile, const char* did);
extern AdvIDocument* adv_idio_open_by_documentid (AdvIDocFile* dfile, const char* did);
extern AdvIDocument* adv_idio_open_nth           (AdvIDocFile* dfile, int n);
extern AdvIDocument* adv_idio_open_by_property   (AdvIDocFile* dfile, void* prev, ...);
extern AdvIDocument* adv_idio_open_by_property_v (AdvIDocFile* dfile, void* prev, va_list va);
extern AdvIDocument* adv_idio_open_by_locator    (const char* loc);
extern void          adv_idio_close              (AdvIDocument* doc);

/*----- basic i/o -----*/

extern int32         adv_idio_read  (AdvIDocument* doc, adv_off_t offset, int32 length, void* buf_arg);
extern int32         adv_idio_write (AdvIDocument* doc, adv_off_t offset, int32 length, const void* buf_arg);

/*----- status -----*/

extern const char*   adv_idio_get_documentid     (AdvIDocument* doc);
extern adv_off_t     adv_idio_get_size           (AdvIDocument* doc);
extern const char*   adv_idio_get_locator        (AdvIDocument* doc);

/*----- porperty operation -----*/

extern void          adv_idio_set_property        (AdvIDocument* doc,
						   const char* key,
						   const char* val);
extern void          adv_idio_set_property_int32  (AdvIDocument* doc,
						   const char* key,
						   int32 val);
extern void          adv_idio_set_property_float64(AdvIDocument* doc,
						   const char* key,
						   float64 val);
extern void          adv_idio_unset_nth_property  (AdvIDocument* doc,
						   int n);

extern const char*   adv_idio_get_property        (AdvIDocument* doc,
						   const char* key);
extern bool          adv_idio_get_nth_property    (AdvIDocument* doc,
						   int n,
						   char* key, int keysize,
						   char* val, int valsize);
extern bool          adv_idio_get_property_int32  (AdvIDocument* doc,
						   const char* key,
						   int32* val);
extern bool          adv_idio_get_property_float64(AdvIDocument* doc,
						   const char* key,
						   float64* val);
extern bool          adv_idio_check_property_v (AdvIDocument* doc, va_list va);
extern bool          adv_idio_check_property   (AdvIDocument* doc,
						...);

/*--- read ---*/

extern int32         adv_idio_read_octet          (AdvIDocument* doc,
						   adv_off_t offset,
						   int32 length,
						   octet* buf);
extern int32         adv_idio_read_string_length  (AdvIDocument* doc,
						   adv_off_t offset);
extern int32         adv_idio_read_string         (AdvIDocument* doc,
						   adv_off_t offset,
						   char* val);
extern int32         adv_idio_read_int8           (AdvIDocument* doc,
						   adv_off_t offset,
						   int8* val);
extern int32         adv_idio_read_int8v          (AdvIDocument* doc,
						   adv_off_t offset,
						   int count,
						   int8* val);
extern int32         adv_idio_read_int16          (AdvIDocument* doc,
						   adv_off_t offset,
						   int16* val);
extern int32         adv_idio_read_int16v         (AdvIDocument* doc,
						   adv_off_t offset,
						   int count,
						   int16* val);
extern int32         adv_idio_read_int32          (AdvIDocument* doc,
						   adv_off_t offset,
						   int32* val);
extern int32         adv_idio_read_int32v         (AdvIDocument* doc,
						   adv_off_t offset,
						   int count,
						   int32* val);
#ifdef HAVE_ADVSYS_INT64
extern int32         adv_idio_read_int64          (AdvIDocument* doc,
						   adv_off_t offset,
						   int64* val);
extern int32         adv_idio_read_int64v         (AdvIDocument* doc,
						   adv_off_t offset,
						   int count,
						   int64* val);
#else
extern int32         adv_idio_read_int64          (AdvIDocument* doc,
						   adv_off_t offset,
						   int32* val);
extern int32         adv_idio_read_int64v         (AdvIDocument* doc,
						   adv_off_t offset,
						   int count,
						   int32* val);
#endif
extern int32         adv_idio_read_float32        (AdvIDocument* doc,
						   adv_off_t offset,
						   float32* val);
extern int32         adv_idio_read_float32v       (AdvIDocument* doc,
						   adv_off_t offset,
						   int count,
						   float32* val);
extern int32         adv_idio_read_float64        (AdvIDocument* doc,
						   adv_off_t offset,
						   float64* val);
extern int32         adv_idio_read_float64v       (AdvIDocument* doc,
						   adv_off_t offset,
						   int count,
						   float64* val);

/*----- write -----*/

extern int32         adv_idio_write_octet         (AdvIDocument* doc,
						   adv_off_t offset,
						   int32 length,
						   const octet* buf);
extern int32         adv_idio_write_string        (AdvIDocument* doc,
						   adv_off_t offset,
						   const char* val);
extern int32         adv_idio_write_int8          (AdvIDocument* doc,
						   adv_off_t offset,
						   int8 val);
extern int32         adv_idio_write_int8v         (AdvIDocument* doc,
						   adv_off_t offset,
						   int num,
						   const int8* val);
extern int32         adv_idio_write_int16         (AdvIDocument* doc,
						   adv_off_t offset,
						   int16 val);
extern int32         adv_idio_write_int16v        (AdvIDocument* doc,
						   adv_off_t offset,
						   int num,
						   const int16* val);
extern int32         adv_idio_write_int32         (AdvIDocument* doc,
						   adv_off_t offset,
						   int32 val);
extern int32         adv_idio_write_int32v        (AdvIDocument* doc,
						   adv_off_t offset,
						   int num,
						   const int32* val);
#ifdef HAVE_ADVSYS_INT64
extern int32         adv_idio_write_int64         (AdvIDocument* doc,
						   adv_off_t offset,
						   int64 val);
extern int32         adv_idio_write_int64v        (AdvIDocument* doc,
						   adv_off_t offset,
						   int num,
						   const int64* val);
#else
extern int32         adv_idio_write_int64         (AdvIDocument* doc,
						   adv_off_t offset,
						   int32 val);
extern int32         adv_idio_write_int64v        (AdvIDocument* doc,
						   adv_off_t offset,
						   int num,
						   const int32* val);
#endif
extern int32         adv_idio_write_float32       (AdvIDocument* doc,
						   adv_off_t offset,
						   float32 val);
extern int32         adv_idio_write_float32v      (AdvIDocument* doc,
						   adv_off_t offset,
						   int num,
						   const float32* val);
extern int32         adv_idio_write_float64       (AdvIDocument* doc,
						   adv_off_t offset,
						   float64 val);
extern int32         adv_idio_write_float64v      (AdvIDocument* doc,
						   adv_off_t offset,
						   int num,
						   const float64* val);
#ifdef __cplusplus
};
#endif

#endif

