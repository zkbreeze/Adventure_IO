/* advsys-config.h.  Generated automatically by configure.  */
/* advsys-config.h.in.  Generated automatically from configure.in by autoheader.  */

#ifndef _advsys_config_h_included_
#define _advsys_config_h_included_


/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define to `long' if <sys/types.h> doesn't define.  */
/* #undef off_t */

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if your processor stores words with the most significant
   byte first (like Motorola and SPARC, unlike Intel and VAX).  */
/* #undef WORDS_BIGENDIAN */

#define MAJOR_VERSION "1.2"
#define MINOR_VERSION "20060202-6"
/* #undef DEBUG */
/* #undef DEBUG_MEMSTAT */
#define HAVE_CXX 1
#define CXX_HAVE_EXCEPTION 1
#define CXX_HAVE_NAMESPACE 1
#define HAVE_SYSTYPES_H 1
/* #undef HAVE_SYSTYPE_INT8 */
/* #undef HAVE_SYSTYPE_UINT8 */
/* #undef HAVE_SYSTYPE_INT16 */
/* #undef HAVE_SYSTYPE_UINT16 */
/* #undef HAVE_SYSTYPE_INT32 */
/* #undef HAVE_SYSTYPE_UINT32 */
/* #undef HAVE_SYSTYPE_INT64 */
/* #undef HAVE_SYSTYPE_UINT64 */
/* #undef HAVE_SYSTYPE_FLOAT32 */
/* #undef HAVE_SYSTYPE_FLOAT64 */
/* #undef HAVE_SYSTYPE_FLOAT128 */

/* #undef HAVE_MPI */
/* #undef socklen_t */
#define USE_ZLIB 1

#define HAVE_GTK 1
/* #undef HAVE_MICO */
/* #undef HAVE_MICO_2_3_X */
/* #undef HAVE_ORBIT */
#define HAVE_CURSES 1
/* #undef HAVE_ORB */

#define HAVE_FUNC_SNPRINTF 1
#define HAVE_FUNC_REALPATH 1

/* #undef MACHINE_SR */

/* Define if <stdarg.h> defines va_copy. */
/* #undef HAVE_VA_COPY */

/* Define if <stdarg.h> defines __va_copy. */
#define HAVE___VA_COPY 1

/* The number of bytes in a bool.  */
#define SIZEOF_BOOL 1

/* The number of bytes in a double.  */
#define SIZEOF_DOUBLE 8

/* The number of bytes in a float.  */
#define SIZEOF_FLOAT 4

/* The number of bytes in a int.  */
#define SIZEOF_INT 4

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* The number of bytes in a long double.  */
#define SIZEOF_LONG_DOUBLE 12

/* The number of bytes in a long long.  */
#define SIZEOF_LONG_LONG 8

/* The number of bytes in a short.  */
#define SIZEOF_SHORT 2

/* The number of bytes in a size_t.  */
#define SIZEOF_SIZE_T 4

/* Name of package */
#define PACKAGE "AdvIO"

/* Version number of package */
#define VERSION "1.2"

#endif  /*  _advsys_config_h_included_ */
