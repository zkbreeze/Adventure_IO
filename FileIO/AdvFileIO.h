/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#ifndef _AdvFileIO_h_included_
#define _AdvFileIO_h_included_

#include <Adv/AdvTypes.h>
#include <fcntl.h>
#include <stdarg.h>

#ifdef HAVE_ADVSYS_INT64
typedef int64 adv_off_t;
#else
typedef int32 adv_off_t;
#endif
typedef struct _AdvFile AdvFile;

#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END   };
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

EXTERN_C_BEGIN

/*
  in case type of retval is bool,
  true: success, no error, ok
  false: fail, ng, error
*/  

/*------------------------------------------------------------
  file I/O
  ------------------------------------------------------------*/ 

extern AdvFile*  adv_fio_open(const char* filename, const char* mode);
extern AdvFile*  adv_fio_create(const char* filename);
extern void      adv_fio_close(AdvFile* file);
extern int32     adv_fio_read(AdvFile* file, adv_off_t offset, int32 length, void* buf);
extern int32     adv_fio_write(AdvFile* file, adv_off_t offset, int32 length, const void* buf);
extern void      adv_fio_lseek(AdvFile* file, adv_off_t offset, bool relative);
extern adv_off_t adv_fio_get_size(AdvFile* file);

extern int32 adv_fio_read_int8          (AdvFile* file, adv_off_t offset, int8* val);
extern int32 adv_fio_read_int16         (AdvFile* file, adv_off_t offset, int16* val);
extern int32 adv_fio_read_int32         (AdvFile* file, adv_off_t offset, int32* val);
#ifdef HAVE_ADVSYS_INT64      
extern int32 adv_fio_read_int64         (AdvFile* file, adv_off_t offset, int64* val);
#else
extern int32 adv_fio_read_int64         (AdvFile* file, adv_off_t offset, int32* val);
#endif
extern int32 adv_fio_read_float32       (AdvFile* file, adv_off_t offset, float32* val);
extern int32 adv_fio_read_float64       (AdvFile* file, adv_off_t offset, float64* val);
extern int32 adv_fio_read_string_length (AdvFile* file, adv_off_t offset);
extern int32 adv_fio_read_string        (AdvFile* file, adv_off_t offset, char* val);

extern int32 adv_fio_write_int8         (AdvFile* file, adv_off_t offset, int8 val);
extern int32 adv_fio_write_int16        (AdvFile* file, adv_off_t offset, int16 val);
extern int32 adv_fio_write_int32        (AdvFile* file, adv_off_t offset, int32 val);
#ifdef HAVE_ADVSYS_INT64
extern int32 adv_fio_write_int64        (AdvFile* file, adv_off_t offset, int64 val);
#else
extern int32 adv_fio_write_int64        (AdvFile* file, adv_off_t offset, int32 val);
#endif
extern int32 adv_fio_write_float32      (AdvFile* file, adv_off_t offset, float32 val);
extern int32 adv_fio_write_float64      (AdvFile* file, adv_off_t offset, float64 val);
extern int32 adv_fio_write_string       (AdvFile* file, adv_off_t offset, const char* val);

extern int32 adv_fio_fprintf            (AdvFile* file, const char* fmt, ...);
extern int32 adv_fio_fscanf             (AdvFile* file, const char* fmt, ...);
extern int32 adv_fio_fprintf2           (AdvFile* file, adv_off_t offset, const char* fmt, ...);
extern int32 adv_fio_fscanf2            (AdvFile* file, adv_off_t offset, const char* fmt, ...);
extern int32 adv_fio_vfprintf2          (AdvFile* file, adv_off_t offset, const char* fmt, va_list va);
extern int32 adv_fio_vfscanf2           (AdvFile* file, adv_off_t offset, const char* fmt, va_list va);

EXTERN_C_END

#endif
