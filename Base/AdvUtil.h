/***********************************************************
Copyright (C) 2000,2001,2002,2006 
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

/*
 * utility routines
 *
 */

#ifndef _AdvUtil_h_included_
#define _AdvUtil_h_included_

#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>
#include <limits.h>

#include "advsys-config.h"
#include "AdvTypes.h"

#ifdef __cplusplus
#define _extern_c_begin extern "C" {
#define _extern_c_end   }
#else
#define _extern_c_begin
#define _extern_c_end
#endif

_extern_c_begin

/*============================================================
  option analyze
  ============================================================*/

extern bool adv_opt_get_flag   (int* ac, char*** av, const char* name1, const char* name2);
extern bool adv_opt_get_int    (int* ac, char*** av, const char* name1, const char* name2, int* val);
extern bool adv_opt_get_double (int* ac, char*** av, const char* name1, const char* name2, double* val);
extern bool adv_opt_get_string (int* ac, char*** av, const char* name1, const char* name2, char** val);

/*============================================================
  doubly-linked-list
  ============================================================*/

typedef struct _AList
{
  struct _AList* next;
  struct _AList* prev;
  void* data;
} AList;

AList* a_list_new      (void);
void   a_list_destroy  (AList* list);
AList* a_list_insert   (AList* list, int offset, void* d);
AList* a_list_append   (AList* list, void* d);
AList* a_list_prepend  (AList* list, void* d);
AList* a_list_remove   (AList* list);
int    a_list_length   (AList* list);
AList* a_list_first    (AList* list);
AList* a_list_last     (AList* list);
AList* a_list_nth      (AList* list, int n);
void*  a_list_nth_data (AList* list, int n);

/*============================================================
  binary-tree
  ============================================================*/

typedef struct _ATreeNode ATreeNode;
typedef struct _ATree     ATree;

struct _ATree
{
  ATreeNode* root;
  int (*cmpfunc)(void*, void*);
};

ATree* a_tree_new    (int (*cmpfunc)(void*, void*));
ATree* a_tree_insert (ATree* tree, void* data, void* key);
void   a_tree_destroy(ATree* tree);
void*  a_tree_lookup (ATree* tree, void* key);

/*============================================================
  memory allocation
  ============================================================*/

extern void*  a_malloc_real (size_t size, const char* file, int line);
extern void   a_free_real   (void*);
extern void   a_memory_profile_real(void);
extern size_t a_memory_maxused(void);
extern size_t a_memory_alloced(void);

#ifdef DEBUG_MEMSTAT
#ifdef sgi
/* a_malloc_real caus bus error on IRIX */
#else
#define A_MEMORY_STAT 2
#endif
#else
#undef A_MEMORY_STAT
#endif

#ifdef A_MEMORY_STAT
#define a_new(type)          (type*)a_malloc_real (sizeof(type),   __FILE__, __LINE__)
#define a_new_array(type, c) (type*)a_malloc_real (sizeof(type)*(c), __FILE__, __LINE__)
#define a_malloc(size)              a_malloc_real (size,           __FILE__, __LINE__)
#define a_free(ptr)                 a_free_real   (ptr);
#define a_memory_profile()          a_memory_profile_real()
#else
#define a_new(type)          (type*)malloc (sizeof(type))
#define a_new_array(type, c) (type*)malloc (sizeof(type)*(c))
#define a_malloc(size)              malloc (size)
#define a_free(ptr)                 free   (ptr)
#define a_memory_profile()
#endif

/*============================================================
  int64 emulation
  ============================================================*/

#if 0
#ifndef HAVE_ADVSYS_INT64
int64 adv_int64_add(int64 a, int64 b);		  /* emulate a+b */
int64 adv_int64_sub(int64 a, int64 b);		  /* emulate a-b */
int64 adv_int64_set(int32 hi, uint32 lo);	  /* make int64 from hi&lo */
#endif
#endif

/*============================================================
  va_copy emulation
  ============================================================*/
#if defined (HAVE_VA_COPY)
#elif defined (HAVE___VA_COPY)
#define va_copy(d,s)  __va_copy((d),(s))
#else
#define va_copy(d,s)  (d) = (s)
#endif

_extern_c_end

#endif /* _AdvUtil_h_includes_ */

/* [eof] */

