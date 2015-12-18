/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include <Adv/AdvUtil.h>
#include "AdvMDocument.h"

#include <string.h>

#define swap_octet(a, b) {octet c; c = a; a = b; b = c;}

/*============================================================
  memory document
  ============================================================*/

int32
adv_mdio_read_octet (const octet* mdoc, adv_off_t offset, int32 length, octet* buf)
{
  memcpy(buf, mdoc+(int)offset, length);
  return length;
}

int32
adv_mdio_read_int8  (const octet* mdoc, adv_off_t offset, int8* val)
{
  *val = *((int8*)(mdoc+offset));
  return sizeof(int8);
}

int32
adv_mdio_read_int16 (const octet* mdoc, adv_off_t offset, int16* val)
{
  int32 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_mdio_read_octet(mdoc, offset, sizeof(int16), (octet*)val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[1]);
#endif
  return r;
}

int32
adv_mdio_read_int32 (const octet* mdoc, adv_off_t offset, int32* val)
{
  int32 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_mdio_read_octet(mdoc, offset, sizeof(int32), (octet*)val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return r;
}

#ifdef HAVE_ADVSYS_INT64
int32
adv_mdio_read_int64(const octet* mdoc, adv_off_t offset, int64* val)
{
  int32 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_mdio_read_octet(mdoc, offset, sizeof(int64), (octet*)val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[7]);
  swap_octet(b[1], b[6]);
  swap_octet(b[2], b[5]);
  swap_octet(b[3], b[4]);
#endif
  return r;
}
#else
int32
adv_mdio_read_int64(const octet* mdoc, adv_off_t offset, int32* val)
{
  int32 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_mdio_read_octet(mdoc, offset, sizeof(int32), (octet*)val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return (int32)8;
}
#endif

int32
adv_mdio_read_float32(const octet* mdoc, adv_off_t offset, float32* val)
{
  int32 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_mdio_read_octet(mdoc, offset, sizeof(float32), (octet*)val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return r;
}

int32
adv_mdio_read_float64(const octet* mdoc, adv_off_t offset, float64* val)
{
  int32 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_mdio_read_octet(mdoc, offset, sizeof(float64), (octet*)val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[7]);
  swap_octet(b[1], b[6]);
  swap_octet(b[2], b[5]);
  swap_octet(b[3], b[4]);
#endif
  return r;
}
