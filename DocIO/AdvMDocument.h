/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#ifndef _AdvMDocument_h_included_
#define _AdvMDocument_h_included_

#include <Adv/AdvFileIO.h>

#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END   };
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

EXTERN_C_BEGIN

/*------------------------------------------------------------
  memory document
  ------------------------------------------------------------*/

extern int32 adv_mdio_read_octet     (const octet* mdoc, adv_off_t offset, int32 length, octet* buf);
extern int32 adv_mdio_read_int8      (const octet* mdoc, adv_off_t offset, int8* val);
extern int32 adv_mdio_read_int8v     (const octet* mdoc, adv_off_t offset, int num, int8* val);
extern int32 adv_mdio_read_int16     (const octet* mdoc, adv_off_t offset, int16* val);
extern int32 adv_mdio_read_int16v    (const octet* mdoc, adv_off_t offset, int num, int16* val);
extern int32 adv_mdio_read_int32     (const octet* mdoc, adv_off_t offset, int32* val);
extern int32 adv_mdio_read_int32v    (const octet* mdoc, adv_off_t offset, int num, int32* val);
#ifdef HAVE_ADVSYS_INT64
extern int32 adv_mdio_read_int64     (const octet* mdoc, adv_off_t offset, int64* val);
extern int32 adv_mdio_read_int64v    (const octet* mdoc, adv_off_t offset, int num, int64* val);
#else
extern int32 adv_mdio_read_int64     (const octet* mdoc, adv_off_t offset, int32* val);
extern int32 adv_mdio_read_int64v    (const octet* mdoc, adv_off_t offset, int num, int32* val);
#endif
extern int32 adv_mdio_read_float32   (const octet* mdoc, adv_off_t offset, float32* val);
extern int32 adv_mdio_read_float32v  (const octet* mdoc, adv_off_t offset, int num, float32* val);
extern int32 adv_mdio_read_float64   (const octet* mdoc, adv_off_t offset, float64* val);
extern int32 adv_mdio_read_float64v  (const octet* mdoc, adv_off_t offset, int num, float64* val);

extern int32 adv_mdio_write_octet    (const octet* mdoc, adv_off_t offset, int32 length, const octet* buf);
extern int32 adv_mdio_write_int8     (const octet* mdoc, adv_off_t offset, int8 val);
extern int32 adv_mdio_write_int8v    (const octet* mdoc, adv_off_t offset, int num, const int8* val);
extern int32 adv_mdio_write_int16    (const octet* mdoc, adv_off_t offset, int16 val);
extern int32 adv_mdio_write_int16v   (const octet* mdoc, adv_off_t offset, int num, const int16* val);
extern int32 adv_mdio_write_int32    (const octet* mdoc, adv_off_t offset, int32 val);
extern int32 adv_mdio_write_int32v   (const octet* mdoc, adv_off_t offset, int num, const int32* val);
#ifdef HAVE_ADVSYS_INT64
extern int32 adv_mdio_write_int64    (const octet* mdoc, adv_off_t offset, int64 val);
extern int32 adv_mdio_write_int64v   (const octet* mdoc, adv_off_t offset, int num, const int64* val);
#else
extern int32 adv_mdio_write_int64    (const octet* mdoc, adv_off_t offset, int32 val);
extern int32 adv_mdio_write_int64v   (const octet* mdoc, adv_off_t offset, int num, const int32* val);
#endif
extern int32 adv_mdio_write_float32  (const octet* mdoc, adv_off_t offset, float32 val);
extern int32 adv_mdio_write_float32v (const octet* mdoc, adv_off_t offset, int num, const float32* val);
extern int32 adv_mdio_write_float64  (const octet* mdoc, adv_off_t offset, float64 val);
extern int32 adv_mdio_write_float64v (const octet* mdoc, adv_off_t offset, int num, const float64* val);

EXTERN_C_END

#endif
