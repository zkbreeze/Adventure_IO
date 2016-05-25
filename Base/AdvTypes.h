/***********************************************************
Copyright (C) 2000,2001,2002,2006 
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

/*
 * basically difinitions
 *
 */

#ifndef _AdvTypes_h_included_
#define _AdvTypes_h_included_

#include "advsys-config.h"
#ifdef HAVE_SYSTYPES_H
#include <sys/types.h>
#endif
#include <limits.h>

/*============================================================
  boolean type
  ============================================================*/

// #ifdef __cplusplus
// /*---------- C++ ----------*/

// #ifndef HAVE_CXX
// #error  c++ support is disabled. Please recompile AdvIO!!
// #endif

// #if SIZEOF_BOOL != 0

// /* use C++-builtin boolean (do nothing) */

// #elif USE_STL

// /* use stl's boolean */
// #include <stl.h>

// #else

// /* use own boolean */
// typedef int bool_new;
// const bool_new false = 0;
// const bool_new true = !false;

// #endif

// #else
// /*---------- C ----------*/

// typedef int bool_new;
// #define false 0
// #define true 1

// #endif /* __cplusplus */

#define false 0
#define true 1

/*============================================================
  bitlength specified integer
  ============================================================*/

#if CHAR_BIT != 8
#error CHAR_BIT must be 8!!!
#endif

#ifndef HAVE_SYSTYPE_INT8
typedef char               int8;
#endif

#ifndef HAVE_SYSTYPE_UINT8
typedef unsigned char      uint8;
#endif

#ifndef HAVE_SYSTYPE_INT16
#if SIZEOF_SHORT == 2
typedef short              int16;
#elif SIZEOF_INT == 2
typedef int                int16;
#endif
#endif

#ifndef HAVE_SYSTYPE_UINT16
#if SIZEOF_SHORT == 2
typedef unsigned short     uint16;
#elif SIZEOF_INT == 2
typedef unsigned int       uint16;
#endif
#endif

#ifndef HAVE_SYSTYPE_INT32
#if SIZEOF_INT == 4
typedef int                int32;
#elif SIZEOF_LONG == 4
typedef long               int32;
#endif
#endif

#ifndef HAVE_SYSTYPE_UINT32
#if SIZEOF_INT == 4
typedef unsigned int       uint32;
#elif SIZEOF_LONG == 4
typedef unsigned long      uint32;
#endif
#endif

#ifdef HAVE_SYSTYPE_INT64
#define HAVE_ADVSYS_INT64
#else
#if SIZEOF_LONG_LONG == 8
#define HAVE_ADVSYS_INT64
typedef long long          int64;
#endif
#endif

#ifndef HAVE_SYSTYPE_UINT64
#if SIZEOF_LONG_LONG == 8
typedef unsigned long long uint64;
#endif
#endif

/*============================================================
  bitlength specified floating point
============================================================*/

#if SIZEOF_FLOAT == 4
typedef float              float32;
#elif SIZEOF_DOUBLE == 4
typedef double             float32;
#endif

#if SIZEOF_DOUBLE == 8
typedef double             float64;
#endif

#if SIZEOF_LONG_DOUBLE == 16
typedef long double        float128;
#endif

/*============================================================
  other types
  ============================================================*/

typedef unsigned char      byte;
typedef unsigned char      octet;

#endif /* _AdvTypes_h_included_ */
