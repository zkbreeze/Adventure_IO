/***********************************************************
Copyright (C) 2000,2001,2002,2006 
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

/*
 * datafile I/O
 *
 */

#include <stdio.h>
#include "AdvUtil.h"
#include <string.h>

/*============================================================
  option analyze
  ============================================================*/

static
void
adv_opt_remove (int* ac,
		char*** av,
		int pos,
		int count)
{
  int i;
  if (count > 0)
    {
      for (i = pos; i < *ac-count; i++)
	(*av)[i] = (*av)[i+count];
      *ac -= count;
    }
}


/*
  const char* name1: short name (-o, -o val)
  const char* name2: long name (--option, --option=value)
  bool remove: remove the option form `av', `ac' or not
  return: -1=notfound, other=index of position in `av'
 */

static
int
adv_opt_get_val (int* ac,
		 char*** av,
		 const char* name1,
		 const char* name2,
		 char** val,
		 bool remove)
{
  int i;
  char opt[128];
  int len;
  if (!name1 && !name2)
    return -1;					  /* have no sense */
  for (i = 1; i < *ac; i++)
    {
      if (name1)
	{
	  sprintf(opt, "-%s", name1);
	  if (strcmp((*av)[i], opt) == 0)
	    {
	      if (val)
		*val = (*av)[i+1];
	      if (remove)
		{
		  if (val)
		    adv_opt_remove(ac, av, i, 2);
		  else
		    adv_opt_remove(ac, av, i, 1);
		}
	      return i;
	    }
	}
      if (name2)
	{
	  sprintf(opt, "--%s", name2);
	  len = strlen(opt);
	  if (strncmp((*av)[i], opt, len) == 0)
	    {
	      if ((*av)[i][len] == '\0')	  /* like --option */
		{
		  if (val)
		    *val = NULL;
		  if (remove)
		    adv_opt_remove(ac, av, i, 1);
		  return i;
		}
	      else if ((*av)[i][len] == '=')	  /* like --option=val */
		{
		  if (val)
		    *val = (*av)[i]+len+1;
		  if (remove)
		    adv_opt_remove(ac, av, i, 1);
		  return i;
		}
	    }
	}
    }
  if (val)
    *val = NULL;
  return -1;					  /* couldnt find */
}

bool
adv_opt_get_flag (int* ac,
		  char*** av,
		  const char* name1,
		  const char* name2)
{
  int i;
  i = adv_opt_get_val(ac, av, name1, name2, NULL, true);
  return i > 0 ? true : false;
}

bool
adv_opt_get_int (int* ac,
		 char*** av,
		 const char* name1,
		 const char* name2,
		 int* val)
{
  bool found;
  char* v;
  found = adv_opt_get_val(ac, av, name1, name2, &v, true);
  if (found < 0 || v == NULL)
    return false;
  else
    {
      *val = atoi(v);
      return true;
    }
}

bool
adv_opt_get_double (int* ac,
		    char*** av,
		    const char* name1,
		    const char* name2,
		    double* val)
{
  bool found;
  char* v;
  found = adv_opt_get_val(ac, av, name1, name2, &v, true);
  if (found < 0 || v == NULL)
    return false;
  else
    {
      *val = strtod(v, NULL);
      return true;
    }
}

bool
adv_opt_get_string (int* ac,
		    char*** av,
		    const char* name1,
		    const char* name2,
		    char** val)
{
  bool found;
  char* v;
  found = adv_opt_get_val(ac, av, name1, name2, &v, true);
  if (found < 0 || v == NULL)
    return false;
  else
    {
      *val = v;
      return true;
    }
}

/*============================================================
  binary-tree
  ============================================================*/

struct _ATreeNode
{
  ATreeNode* left;
  ATreeNode* right;
  void* data;
  void* key;
};

static ATreeNode* a_tree_node_new    (void* data, void* key);
static ATreeNode* a_tree_node_insert (ATree* tree, ATreeNode* node, void* data, void* key);
static void       a_tree_node_destroy(ATreeNode* node);
static void*      a_tree_node_lookup (ATree* tree, ATreeNode* node, void* key);

static ATreeNode* a_tree_node_new(void* data, void* key)
{
  ATreeNode* nnode;
  nnode = a_new(ATreeNode);
  nnode->left = NULL;
  nnode->right = NULL;
  nnode->data = data;
  nnode->key = key;
  return nnode;
}

static ATreeNode* a_tree_node_insert(ATree* tree, ATreeNode* node, void* data, void* key)
{
  int cmp;
  if (!node)
    {
      return a_tree_node_new(data, key);
    }
  cmp = tree->cmpfunc(key, node->key);
  if (cmp == 0)
    {
      /* duplicated did */
      node->data = data;
      return node;
    }
  if (cmp < 0)
    {
      if (node->left)
	node->left = a_tree_node_insert(tree, node->left, data, key);
      else
	node->left = a_tree_node_new(data, key);
    }
  else if (cmp > 0)
    {
      if (node->right)
	node->right = a_tree_node_insert(tree, node->right, data, key);
      else
	node->right = a_tree_node_new(data, key);
    }
  return node;
}

void a_tree_node_destroy(ATreeNode* node)
{
  if (node->left)
    {
      a_tree_node_destroy(node->left);
      a_free(node->left);
      node->left = NULL;
    }
  if (node->right)
    {
      a_tree_node_destroy(node->right);
      a_free(node->right);
      node->right = NULL;
    }
}

void* a_tree_node_lookup(ATree* tree, ATreeNode* node, void* key)
{
  int cmp;
  if (!node)
    return NULL;
  cmp = tree->cmpfunc(key, node->key);
  if (cmp == 0)
    return node->data;
  if (cmp < 0)
    {
      if (node->left)
	return a_tree_node_lookup(tree, node->left, key);
    }
  else if (cmp > 0)
    {
      if (node->right)
	return a_tree_node_lookup(tree, node->right, key);
    }
  return NULL;
}

ATree* a_tree_new(int (*cmpfunc)(void*, void*))
{
  ATree* tree;
  tree = a_new(ATree);
  tree->root = NULL;
  tree->cmpfunc = cmpfunc;

	return tree; /* Yonemura.N in 7.Feb.2001 */
}

ATree* a_tree_insert(ATree* tree, void* data, void* key)
{
  tree->root = a_tree_node_insert(tree, tree->root, data, key);
  return tree;
}

void a_tree_destroy(ATree* tree)
{
  if (tree->root)
    {
      a_tree_node_destroy(tree->root);
      a_free(tree->root);
    }
  a_free(tree);
}

void* a_tree_lookup(ATree* tree, void* key)
{
  return a_tree_node_lookup(tree, tree->root, key);
}

void a_tree_node_print(ATreeNode* node, int level)
{
  if (node->left)
    a_tree_node_print(node->left, level+1);
  printf("%d %p\n", level, node->key);
  if (node->right)
    a_tree_node_print(node->right, level+1);
}


/*============================================================
  doubly-linked-list operation
  ============================================================*/

AList*
a_list_new()
{ return NULL; }

/*------------------------------------------------------------
  destroy the list
  (arg)
  list:  list to destroy
  (comment)
  each data should be operated ( free, or delete ) before this.
  ------------------------------------------------------------*/

void
a_list_destroy(AList* list)
{
  AList* l;
  AList* m;
  for (l = list; l != NULL; l = m)
    {
      m = l->next;
      free(l);
    }
}

/*
  insert a new data to the list

  (arg)
  list:   new item willbe inserted to this list
  offset: where tobe insert
  d:      what to be insert
  (ret)
  modified list
  (ex.)
  list = a_list_insert( list, 1, data );
*/

AList*
a_list_insert(AList* list, int offset, void* d)
{
  AList* n;
  if (list == NULL)
    {
      n = (AList*)malloc(sizeof(AList));
      n->data = d;
      n->next = NULL;
      n->prev = NULL;
    }
  else
    {
      AList* l = offset == 0 ? list : a_list_nth(list, offset);
      if (!l)
	n = NULL;
      else
	{
	  n = (AList*)malloc(sizeof(AList));
	  n->data = d;
	  n->next = l;
	  n->prev = l->prev;
	  if ( l->prev )
	    l->prev->next = n;
	  l->prev = n;
	}
    }
  return n;
}

/*
  append a new data to the list

  (arg)
  list: new item willbe appended to this.
  d:    what to be appended
  (ret)
  modified list
  (ex.)
  list = a_list_append( list, data );
  list = a_list_append( NULL, data );   // on initial
*/

AList*
a_list_append(AList* list, void* d)
{
  AList* n = (AList*)malloc(sizeof(AList));
  n->data = d;
  n->next = NULL;
  if (list)
    { 
      AList* l = a_list_last(list);
      n->prev = l;
      l->next = n;
      return list;
    }
  else
    {
      n->prev = NULL;
      return n;
    }
}

/*
  append a new data to the list

  (arg)
  list: new item willbe prepended to this.
  d:    what to be prepended
  (ret)
  modified list
*/

AList*
a_list_prepend(AList* list, void* d)
{
  AList* n = (AList*)malloc(sizeof(AList));
  n->data = d;
  n->prev = NULL;
  if (list)
    { 
      n->next = list;
      list->prev = n;
    }
  else
    n->next = NULL;
  return n;
}

/*
  length of list

  (arg)
  list: list
  (ret)
  length of list
*/

int
a_list_length(AList* list)
{
  int length = 0;
  while (list)
    {
      length++;
      list = list->next;
    }
  return length;
}

/*
  the first chunk of list

  (arg)
  list: list
  (ret)
  the first(head) chunk of the list
*/

AList*
a_list_first(AList* list)
{
  if (list)
    {
      while (list->prev)
	list = list->prev;
      return list;
    }
  else
    return NULL;
}

/*
 the last chunk of list

 (arg)
  list: list
 (ret)
  the last(tail) chunk of the list
*/

AList*
a_list_last(AList* list)
{
  if (list)
    {
      while (list->next)
	list = list->next;
      return list;
    }
  else
    return NULL;
}

/*
  nth node of list

  (arg)
  list: list
  n:    index
  (ret)
  n-th chunk of the list ( `list' is 0th )
*/

AList*
a_list_nth(AList* list, int n)
{
  int i;
  for (i = 0; i < n && list != NULL; i++)
    list = list->next;
  return list;
}

/*
  nth data of list

  (arg)
  list: list
  (ret)
  n-th data of the list
*/

void*
a_list_nth_data(AList* list, int n)
{
  AList* l = a_list_nth(list, n);
  return l ? l->data : NULL;
}

/*
  remove the node from list
  
  (arg)
  node: the node to remove
  (ret)
  resulting list
*/

AList*
a_list_remove(AList* node)
{
  AList* head = a_list_first(node);
  if (head == node)
    head = node->next;
  if (node->next)
    node->next->prev = node->prev;
  if (node->prev)
    node->prev->next = node->next;
  free(node);
  return head;
}

/*============================================================
  memory allocation
  ============================================================*/

#if A_MEMORY_STAT >= 2
static AList* memory_block = NULL;
#endif

static size_t memory_alloced = 0;
static size_t memory_maxused = 0;

#if A_MEMORY_STAT >= 2
static
void**
a_memory_get_chunkbuf(void* p)
{
  int i;
  void** block;
  AList* l;
  l = memory_block;

  while (true)
    {
      if (l == NULL)
	{
	  if (p != NULL)
	    return NULL;
	  block = (void**)malloc(sizeof(void*)*1024);
	  for (i = 0; i < 1024; i++)
	    block[i] = NULL;
	  memory_block = a_list_append(memory_block, block);
	}
      else
	{
	  block = (void**)l->data;
	  l = l->next;
	}
  
  
      for (i = 0; i < 1024; i++)
	if (block[i] == p)
	  return block+i;

    }
}
#endif

void*
a_malloc_real(size_t size, const char* file, int line)
{
  size_t* size_ptr;
  octet* chunk;
  octet* p;
  int file_len;
  char* file_ptr;
  int* line_ptr;
  void** chunk_ptr;
  
  file_len = strlen(file)+1;
  file_len += sizeof(void*)
              - (file_len+sizeof(size_t)+sizeof(int)) % sizeof(void*);
  chunk = (octet*)malloc(sizeof(size_t) + sizeof(int) + file_len + sizeof(void*) + size);
  if (!chunk)
    {
#if SIZEOF_SIZE_T == 4
      fprintf(stderr, "memory alloc %d bytes failed at %s:%d\n", size, file, line);
#elif defined (__linux__)
      fprintf(stderr, "memory alloc %zd bytes failed at %s:%d\n", size, file, line);
#else
      fprintf(stderr, "memory alloc %ld bytes failed at %s:%d\n", size, file, line);
#endif
      abort();
    }
  p = chunk;

  size_ptr = (size_t*)p;
  *size_ptr = size;
  p += sizeof(size_t);

  line_ptr = (int*)p;
  *line_ptr = line;
  p += sizeof(int);

  file_ptr = (char*)p;
  strcpy(file_ptr, file);
  p += file_len;

  chunk_ptr = (void**)p;
  *chunk_ptr = chunk;
  p += sizeof(void*);

  memory_alloced += size;
  if (memory_alloced > memory_maxused)
    memory_maxused = memory_alloced;

#if A_MEMORY_STAT >= 2
  {
    void** p = a_memory_get_chunkbuf(NULL);
    *p = chunk;
  }
#endif

  return p;
}

void
a_free_real(void* ptr)
{
  size_t* size_ptr;
  int* line_ptr;
  char* file;
  void** chunk_ptr;
  octet* p;

  if (ptr)
    {
      p = (octet*)ptr;
      p -= sizeof(void*);
      chunk_ptr = (void**)p;
      p = *chunk_ptr;

      size_ptr  = (size_t*)p;
      p += sizeof(size_t);

      line_ptr = (int*)p;
      p += sizeof(int);
      
      file = (char*)p;

      memory_alloced -= *size_ptr;

#if A_MEMORY_STAT >= 2
      {
	void** p = a_memory_get_chunkbuf(*chunk_ptr);
	*p = NULL;
      }
#endif
      free(*chunk_ptr);
    }
}

#ifdef A_MEMORY_STAT
void
a_memory_profile()
{
  printf("MEMORY: max used %d\n", memory_maxused);
  printf("MEMORY: not released %d\n", memory_alloced);
#if A_MEMORY_STAT >= 2
  {
    AList* l;
    int i;
    void** block;
    for (l = memory_block; l != NULL; l = l->next)
      {
	block = (void**)l->data;
	for (i = 0; i < 1024; i++)
	  {
	    if (block[i] != NULL)
	      {
		octet* p;
		size_t* size_ptr;
		int* line_ptr;
		char* file;
		p = (octet*)block[i];

		size_ptr  = (size_t*)p;
		p += sizeof(size_t);
		
		line_ptr = (int*)p;
		p += sizeof(int);
      
		file = (char*)p;

		printf("MEMORY: not released chunk %p, alloced at %s:%d size %d\n",
		       block[i], file, *line_ptr, *size_ptr);
	      }
	  }
      }
  }
#endif
}
#endif

size_t
a_memory_maxused()
{ return memory_maxused; }

size_t
a_memory_alloced()
{ return memory_alloced; }

