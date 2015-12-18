/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include "AdvFileIO.h"
#include <Adv/AdvUtil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#ifdef DEBUG
#define DEBUG_FP stdout
#endif

#define MACHINE_SR

/*============================================================
  low level file access
  ============================================================*/

struct _AdvFile
{
  char* basename;
  int32 num_part;
  int* part_fd;
  off_t* part_data_size;			  /* part size of actual data */
  off_t* part_size;				  /* part size of file truncated (maybe larger than datasize in writemode) */
  int32 current_part;
  int32 current_page;
  octet* current_buf;				  /* current filebuf */
  size_t current_buf_size;
  int filemode;					  /* 'r' || 'c' || 'w' */
  adv_off_t size;				  /* total size */

  adv_off_t current_off;
#ifdef MACHINE_SR
  bool page_dirty;
#endif
};

static const size_t mapbuf_desired = 1024*256;	  /* desired size of mapbuf */
static size_t       mapbuf_size = 0;		  /* real size of mapbuf */

static int32 partsize = 2*1000*1000*1000;	  /* file's part size */
						  /* large file willbe divided into this size */

static const int sizeof_adv_off_t = 8;

/*------------------------------------------------------------
  adv_file_map
  - maps file into virtual memory (using mmap)
  AdvFile* file: target file
  int32 part: part number
  int32 page: page number
  return: false = ok, true = error
  ------------------------------------------------------------*/
static
bool
adv_fio_file_map(AdvFile* file, int32 part, int32 page)
{
  size_t size;

#ifdef DEBUG
  fprintf (DEBUG_FP, "AdvIO::FileIO::adv_fio_file_map():\n");
  fprintf (DEBUG_FP, "  part=%d, page=%d\n", part, page);
  fprintf (DEBUG_FP, "  file: [%s]\n", file->basename);
  fprintf (DEBUG_FP, "    ->current_part=%d\n", file->current_part);
  fprintf (DEBUG_FP, "    ->current_page=%d\n", file->current_page);
  fprintf (DEBUG_FP, "    ->filemode=%c\n", file->filemode);
#endif

  if (file->current_part == part && file->current_page == page)
    return false;				  /* nothing to do */
  if (part >= file->num_part)
    {
      if (file->filemode == 'r')
	return true;
      else
	{					  /* create new a part */
	  char* newname;
	  newname = (char*)malloc(strlen(file->basename)+10);
	  sprintf(newname, "%s.A%02d", file->basename, part);
	  file->part_fd[part] = open(newname, O_CREAT|O_RDWR|O_TRUNC, 0644);
	  file->part_size[part] = 0;
	  file->part_data_size[part] = 0;
	  file->num_part = part+1;
	}
    }
#ifdef MACHINE_SR
  if (file->page_dirty)
    {
      file->page_dirty = false;
      lseek(file->part_fd[file->current_part], file->current_page*mapbuf_size, SEEK_SET);
      write(file->part_fd[file->current_part], file->current_buf, file->current_buf_size);
    }
#else
  if ((int)(file->current_buf) != -1)
    {
      msync((caddr_t)file->current_buf, file->current_buf_size, MS_ASYNC);
      munmap((caddr_t)file->current_buf, file->current_buf_size); /* unmap old buffer */
    }
#endif
  
  size = mapbuf_size;
  if (page*mapbuf_size > partsize)
    return true;				  /* error (out of partsize) */
  else if ((page+1)*mapbuf_size > partsize)
    size = partsize - page*mapbuf_size;		  /* adjust to partsize */
  if (file->filemode == 'r')
    {
      if (page*mapbuf_size > file->part_size[part])
	return true;				  /* error (out of datasize) */
      else if ((page+1)*mapbuf_size > file->part_size[part])
	size = file->part_size[part] - page*mapbuf_size; /* adjust to datasize */
#ifdef MACHINE_SR
      lseek(file->part_fd[part], page*mapbuf_size, SEEK_SET);
      read(file->part_fd[part], file->current_buf, size);

#ifdef DEBUG
      fprintf (DEBUG_FP, "AdvIO::FileIO::adv_fio_file_map():\n");
      fprintf (DEBUG_FP, "  part=%d, page=%d, size=%zd\n", part, page, size);
#endif

#else
      file->current_buf = (octet*)mmap(NULL,
			               size,
			               PROT_READ,
			               MAP_SHARED,
			               file->part_fd[part],
			               page*mapbuf_size);
#endif
    }
  else
    {
      if (page*mapbuf_size+size > file->part_size[part])
	{
	  ftruncate(file->part_fd[part], page*mapbuf_size+size); /* enlarge real filesize */
	  file->part_size[part] = page*mapbuf_size+size;
	}
#ifdef MACHINE_SR
      lseek(file->part_fd[part], page*mapbuf_size, SEEK_SET);
      read(file->part_fd[part], file->current_buf, size);
#else
      file->current_buf = (octet*)mmap(NULL,
   			               size,
			               PROT_READ|PROT_WRITE,
			               MAP_SHARED,
			               file->part_fd[part],
			               page*mapbuf_size);
#endif
    }
	
  file->current_part = part;
  file->current_page = page;
  file->current_buf_size = size;
  /* printf("mapped offset %d, size %d\n", page*mapbuf_size, size); */
  return false;
}

/*------------------------------------------------------------
  adv_fio_open
  - open a file
  const char* filename: filename to open
  char* mode: filemode ("r" for read, "w" for write(append if exist, ERROR if not exist)
              , "c" for create(truncate to 0 if exist))
  return: opened filehandle
  ------------------------------------------------------------*/

AdvFile*
adv_fio_open(const char* filename, const char* mode)
{
  const int max_parts = 20;
  AdvFile* file;
  file = (AdvFile*)malloc(sizeof(AdvFile));	  /* allocate file structure */
  if (mapbuf_size == 0)
    {
      size_t pagesize = getpagesize();
      mapbuf_size = mapbuf_desired / pagesize * pagesize;

      
#ifdef DEBUG
      fprintf (DEBUG_FP, "AdvIO::FileIO : mapbuf_size=%zd\n", mapbuf_size);
#endif

    }
  file->basename = (char*)malloc(strlen(filename)+1);
  strcpy(file->basename, filename);

  switch (*mode)
    {
    case 'w':
    case 'W':
      file->filemode = 'w';
      break;
    case 'c':
    case 'C':
      file->filemode = 'c';
      break;
    case 'r':
    case 'R':
    default:
      file->filemode = 'r';
      break;
    }
  file->part_fd = (int*)malloc(sizeof(int)*max_parts);
  file->part_size = (off_t*)malloc(sizeof(off_t)*max_parts);
  file->part_data_size = (off_t*)malloc(sizeof(off_t)*max_parts);
  if (file->filemode == 'c')
    {
      file->num_part = 1;
      file->part_fd[0] = open(file->basename, O_RDWR|O_CREAT|O_TRUNC, 0644);
      file->part_size[0] = 0;
      file->part_data_size[0] = 0;

      if ( file->part_fd[0] < 0 ) file->num_part = 0;

    }
  else
    {
      struct stat statbuf;
      char* name;
      int i = 0;
      file->num_part = 0;
      name = (char*)malloc(strlen(file->basename)+10);
      while (true)
	{
	  if (i == 0)
	    strcpy(name, file->basename);
	  else
	    sprintf(name, "%s.A%02d", file->basename, i);
	  if (file->filemode == 'r')
	    file->part_fd[i] = open(name, O_RDONLY);
	  else
	    file->part_fd[i] = open(name, O_RDWR);
	  if (file->part_fd[i] == -1)
	    break;
	  fstat(file->part_fd[i], &statbuf);
	  file->part_size[i]      = statbuf.st_size;
	  file->part_data_size[i] = statbuf.st_size;
	  file->num_part++;

#ifdef DEBUG
	  fprintf (DEBUG_FP, "AdvIO::FileIO : part[%2d] open [%s]\n", i, name);
#endif

	  i++;
	}
      free(name);
    }
  if (file->num_part == 0)
    {
      free(file->part_fd);
      free(file->part_size);
      free(file->part_data_size);
      free(file->basename);
      free(file);
      return NULL;
    }
  file->size = partsize*(file->num_part-1) + file->part_data_size[file->num_part-1];
  file->current_part = -1;
  file->current_page = -1;
#ifdef MACHINE_SR
  file->page_dirty = false;
  file->current_buf = (void*)malloc(mapbuf_size);
#else
  file->current_buf = (void*)-1;
#endif
  adv_fio_file_map(file, 0, 0);
  file->current_off = 0;
  return file;
}

/*------------------------------------------------------------
  adv_fio_create
  - create a file
  - just a alias for adv_fio_open(*, "c"), as you see :-).
  const char* filename: filename to create
  return: created filehandle
  ------------------------------------------------------------*/

AdvFile*
adv_fio_create(const char* filename)
{
  return adv_fio_open(filename, "c");
}

/*------------------------------------------------------------
  adv_fio_close
  - close a file
  AdvFile* file: filehandle to close
  ------------------------------------------------------------*/

void
adv_fio_close(AdvFile* file)
{
  int i;
#ifdef MACHINE_SR
  lseek(file->part_fd[file->current_part], file->current_page*mapbuf_size, SEEK_SET);
  write(file->part_fd[file->current_part], file->current_buf, file->current_buf_size);
#else
  msync((caddr_t)file->current_buf, file->current_buf_size, MS_ASYNC);
  munmap((caddr_t)file->current_buf, file->current_buf_size);
#endif
  for (i = 0; i < file->num_part; i++)
    {
      if (file->filemode != 'r')
	{
	  /* printf("%d %d\n", i, file->part_data_size[i]); */
	  ftruncate(file->part_fd[i], file->part_data_size[i]);
	}
      close(file->part_fd[i]);
    }
#ifdef MACHINE_SR
  free(file->current_buf);
#endif
  free(file->basename);
  free(file->part_fd);
  free(file->part_size);
  free(file->part_data_size);
  free(file);
}

/*------------------------------------------------------------
  adv_fio_read_in_a_page
  ------------------------------------------------------------*/

static
int32
adv_fio_read_in_a_page(AdvFile* file, adv_off_t offset, void* buf, int32 length)
{
  int32 part, partoff, page, pageoff;
  int32 to_read;
  octet* b;

  part = (int32)(offset / partsize);
  partoff = (int32)(offset % partsize);
  page = partoff / mapbuf_size;
  pageoff = partoff % mapbuf_size;
  if (adv_fio_file_map(file, part, page))
    return 0;
  to_read = (length < file->current_buf_size-pageoff)
    ? length : file->current_buf_size-pageoff;
  b = (octet*)buf;
  memcpy(b, file->current_buf+pageoff, to_read);
  file->current_off = offset+to_read;
  return to_read;
}

/*------------------------------------------------------------
  adv_fio_read
  - read data
  AdvFile* file: filehandle to read
  adv_off_t offset: data offset to read
  int32 length: data length to read
  return: bytes of read data
  ------------------------------------------------------------*/

int32
adv_fio_read(AdvFile* file, adv_off_t offset, int32 length, void* buf)
{
  int32 left;
  int32 bytes = -1;

  left = length;
  while (left > 0 && bytes != 0)
    {
      bytes = adv_fio_read_in_a_page(file, offset, buf, left);

#ifdef DEBUG
      fprintf (DEBUG_FP, "left=%d bytes=%d, buf=%ld\n", left, bytes, *((int64 *)buf));
#endif

      buf = (octet*)buf + (size_t)bytes;
      offset += bytes;
      left -= bytes;
    }
  return length - left;
}

/*------------------------------------------------------------
  adv_fio_write_in_a_page
  ------------------------------------------------------------*/

static
int32
adv_fio_write_in_a_page(AdvFile* file, adv_off_t offset, const void* buf, int32 length)
{
  int32 part, partoff, page, pageoff;
  int32 to_write;
  octet* b;

  part = (int32)(offset / partsize);
  partoff = (int32)(offset % partsize);
  page = partoff / mapbuf_size;
  /* printf("Write part %d, page %d\n", part, page); */
  if (adv_fio_file_map(file, part, page))
    return 0;
  pageoff = partoff % mapbuf_size;
  to_write = (length < file->current_buf_size-pageoff)
    ? length : file->current_buf_size-pageoff;
  b = ((octet*)file->current_buf)+pageoff;
  memcpy(b, buf, to_write);
  if ((partoff + to_write) > file->part_data_size[part])
    {
      file->part_data_size[part] = partoff + to_write;
      /* printf("Enlarge part %d, size %d\n", part, partoff + to_write); */
    }
  file->current_off = offset+to_write;
#ifdef MACHINE_SR
  file->page_dirty = true;
#endif
  return to_write;
}

/*------------------------------------------------------------
  adv_fio_write
  - write data
  AdvFile* file: filehandle to write
  adv_off_t offset: data offset to write
  int32 length: data length to write
  return: bytes of wrote data
  ------------------------------------------------------------*/

int32
adv_fio_write(AdvFile* file, adv_off_t offset, int32 length, const void* buf)
{
  int32 left;
  int32 bytes = -1;

  left = length;
  while (left > 0 && bytes != 0)
    {
      bytes = adv_fio_write_in_a_page(file, offset, buf, left);
      buf = (octet*)buf + bytes;
      offset += bytes;
      left -= bytes;
    }
  return length - left;
}

void 
adv_fio_lseek(AdvFile* file, adv_off_t offset, bool relative)
{
  if (relative)
    file->current_off += offset;
  else
    file->current_off = offset;
}

adv_off_t
adv_fio_get_size(AdvFile* file)
{
  /*
   * 20100622 fixed by yodo :
   * type of partsize("int32") cause downcast and return inverted value.
   * force to calculate "adv_off_t" using explicit cast.
   */
  file->size = ((adv_off_t)partsize)*((adv_off_t)(file->num_part-1)) + file->part_data_size[file->num_part-1];

#ifdef DEBUG
  fprintf (DEBUG_FP, "adv_fio_get_size(): \n");
  fprintf (DEBUG_FP, "  ->partsize=%d\n", partsize);
  fprintf (DEBUG_FP, "  ->num_part=%d\n", file->num_part);
  fprintf (DEBUG_FP, "  ->part_data_size=%zd\n", file->part_data_size[file->num_part-1]);
  fprintf (DEBUG_FP, "    ->file->size=%zd\n", file->size);
#endif


  return file->size;
}

/*============================================================
  basic type I/O
  ============================================================*/

#define swap_octet(a, b) {octet c; c = a; a = b; b = c;}

int32
adv_fio_write_int8(AdvFile* file, adv_off_t offset, int8 val)
{
  return adv_fio_write(file, offset, sizeof(int8), (octet*)(&val));
}

int32
adv_fio_write_int16(AdvFile* file, adv_off_t offset, int16 val)
{
  octet* b = (octet*)(&val);
#ifdef WORDS_BIGENDIAN
  swap_octet(b[0], b[1]);
#endif
  return adv_fio_write(file, offset, sizeof(int16), b);
}

int32
adv_fio_write_int32(AdvFile* file, adv_off_t offset, int32 val)
{
  octet* b = (octet*)(&val);
#ifdef WORDS_BIGENDIAN
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return adv_fio_write(file, offset, sizeof(int32), b);
}

#ifdef HAVE_ADVSYS_INT64
int32
adv_fio_write_int64(AdvFile* file, adv_off_t offset, int64 val)
{
  octet* b = (octet*)(&val);
#ifdef WORDS_BIGENDIAN
  swap_octet(b[0], b[7]);
  swap_octet(b[1], b[6]);
  swap_octet(b[2], b[5]);
  swap_octet(b[3], b[4]);
#endif
  return adv_fio_write(file, offset, sizeof(int64), b);
}
#else
int32
adv_fio_write_int64(AdvFile* file, adv_off_t offset, int32 val)
{
  static octet padding[] = {0,0,0,0};
  octet* b = (octet*)(&val);
#ifdef WORDS_BIGENDIAN
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  adv_fio_write(file, offset, sizeof(int32), b);
  adv_fio_write(file, offset+4, 4, padding);
  return (int32)8;
}
#endif

int32
adv_fio_write_float32(AdvFile* file, adv_off_t offset, float32 val)
{
  octet* b = (octet*)(&val);
#ifdef WORDS_BIGENDIAN
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return adv_fio_write(file, offset, sizeof(float32), b);
}

int32
adv_fio_write_float64(AdvFile* file, adv_off_t offset, float64 val)
{
  octet* b = (octet*)(&val);
#ifdef WORDS_BIGENDIAN
  swap_octet(b[0], b[7]);
  swap_octet(b[1], b[6]);
  swap_octet(b[2], b[5]);
  swap_octet(b[3], b[4]);
#endif
  return adv_fio_write(file, offset, sizeof(float64), b);
}

int32
adv_fio_write_string(AdvFile* file, adv_off_t offset, const char* str)
{
  int len = strlen(str);
  int32 bytes = 0;
  bytes += adv_fio_write_int32(file, offset, len);
  bytes += adv_fio_write(file, offset+bytes, len, str);
  return bytes;
}

int32
adv_fio_read_int8(AdvFile* file, adv_off_t offset, int8* val)
{
  return adv_fio_read(file, offset, sizeof(int8), (octet*)val);
}

int32
adv_fio_read_int16(AdvFile* file, adv_off_t offset, int16* val)
{
  int16 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_fio_read(file, offset, sizeof(int16), val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[1]);
#endif
  return r;
}

int32
adv_fio_read_int32(AdvFile* file, adv_off_t offset, int32* val)
{
  int32 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_fio_read(file, offset, sizeof(int32), val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return r;
}

#ifdef HAVE_ADVSYS_INT64
int32
adv_fio_read_int64(AdvFile* file, adv_off_t offset, int64* val)
{
  int32 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_fio_read(file, offset, sizeof(int64), val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[7]);
  swap_octet(b[1], b[6]);
  swap_octet(b[2], b[5]);
  swap_octet(b[3], b[4]);
#endif

#ifdef DEBUG
  {
    octet oct_target;
    int ioct;


    fprintf (DEBUG_FP, "read_int64: offset=%zd\n", offset);

    for (ioct = 0; ioct < 8; ioct++) {
      int bit_pattern[8];

      oct_target = ((octet*)val)[ioct];
      bit_pattern[0] = (oct_target & (octet)128) != 0 ? 1 : 0;
      bit_pattern[1] = (oct_target & (octet) 64) != 0 ? 1 : 0;
      bit_pattern[2] = (oct_target & (octet) 32) != 0 ? 1 : 0;
      bit_pattern[3] = (oct_target & (octet) 16) != 0 ? 1 : 0;
      bit_pattern[4] = (oct_target & (octet)  8) != 0 ? 1 : 0;
      bit_pattern[5] = (oct_target & (octet)  4) != 0 ? 1 : 0;
      bit_pattern[6] = (oct_target & (octet)  2) != 0 ? 1 : 0;
      bit_pattern[7] = (oct_target & (octet)  1) != 0 ? 1 : 0;

      fprintf (DEBUG_FP, "  b[%d]=[%d%d%d%d%d%d%d%d]\n",
	       ioct,
	       bit_pattern[0], bit_pattern[1],
	       bit_pattern[2], bit_pattern[3],
	       bit_pattern[4], bit_pattern[5],
	       bit_pattern[6], bit_pattern[7]);
    }
  }
#endif

  return r;
}
#else
int32
adv_fio_read_int64(AdvFile* file, adv_off_t offset, int32* val)
{
  int32 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_fio_read(file, offset, sizeof(int32), val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return (int32)8;
}
#endif

int32
adv_fio_read_float32(AdvFile* file, adv_off_t offset, float32* val)
{
  int32 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_fio_read(file, offset, sizeof(float32), val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[3]);
  swap_octet(b[1], b[2]);
#endif
  return r;
}

int32
adv_fio_read_float64(AdvFile* file, adv_off_t offset, float64* val)
{
  int32 r;
#ifdef WORDS_BIGENDIAN
  octet* b;
#endif
  r = adv_fio_read(file, offset, sizeof(float64), val);
#ifdef WORDS_BIGENDIAN
  b = (octet*)val;
  swap_octet(b[0], b[7]);
  swap_octet(b[1], b[6]);
  swap_octet(b[2], b[5]);
  swap_octet(b[3], b[4]);
#endif
  return r;
}

int32
adv_fio_read_string(AdvFile* file, adv_off_t offset, char* str)
{
  int len;
  int32 bytes = 0;
  bytes += adv_fio_read_int32(file, offset, &len);
  bytes += adv_fio_read(file, offset+bytes, len, str);
  str[len] = '\0';
  return bytes;
}

int32
adv_fio_read_string_length(AdvFile* file, adv_off_t offset)
{
  int len;
  adv_fio_read_int32(file, offset, &len);
  return len;
}

static
int32
adv_fio_read_string_alloc(AdvFile* file, adv_off_t offset, char** str)
{
  int len;
  int32 bytes = 0;
  bytes += adv_fio_read_int32(file, offset, &len);
  *str = (char*)malloc(len+1);
  bytes += adv_fio_read(file, offset+bytes, len, *str);
  (*str)[len] = '\0';
  return bytes;
}

/*============================================================
  printf & scanf
  ============================================================*/

/*------------------------------------------------------------
  adv_fio_bfprintf
  ------------------------------------------------------------*/

int32
adv_fio_vfprintf(AdvFile* file, adv_off_t offset, const char* fmt, va_list va)
{
  bool done;
  const char* p;
  const char* spec = NULL ;
  const char* length = NULL ;
  adv_off_t bytes = 0;

  done = false;
  p = fmt;
  while (!done)
    {
      while (*p && *p++ != '%')
	;
      while (*p)
	{
	  if (*p == '%')
	    {
	      spec = NULL;
	      break;
	    }
	  length = strchr("Lqlh", *p);
	  if (length)
	    p++;
	  spec = strchr("sdif", *p);
	  p++;
	  if (spec)
	    break;
	}
      if (spec)
	{
	  switch (*spec)
	    {
	    case 'd':
	    case 'i':
#ifdef HAVE_ADVSYS_INT64
	      if (length && (*length == 'L' || *length == 'q'))
		bytes += adv_fio_write_int64(file, offset+bytes, va_arg(va, int64));
	      else
#endif
		bytes += adv_fio_write_int32(file, offset+bytes, va_arg(va, int32));
	      break;
	    case 'f':
	      if (length && *length == 'l')
		bytes += adv_fio_write_float64(file, offset+bytes, va_arg(va, float64));
	      else
		bytes += adv_fio_write_float32(file, offset+bytes, va_arg(va, double));
	    }
	}
      if (*p == '\0')
	done = true;
    }
  return bytes;
}

int32
adv_fio_fprintf(AdvFile* file, const char* fmt, ...)
{
  va_list va;
  adv_off_t bytes;
  va_start(va, fmt);
  bytes = adv_fio_vfprintf(file, file->current_off, fmt, va);
  va_end(va);
  return bytes;
}

int32
adv_fio_fprintf2(AdvFile* file, adv_off_t offset, const char* fmt, ...)
{
  va_list va;
  adv_off_t bytes;
  va_start(va, fmt);
  bytes = adv_fio_vfprintf(file, offset, fmt, va);
  va_end(va);
  return bytes;
}

/*------------------------------------------------------------
  adv_fio_bfscanf
  ------------------------------------------------------------*/

int32
adv_fio_vfscanf(AdvFile* file, adv_off_t offset, const char* fmt, va_list va)
{
  bool done;
  const char* p;
  const char* spec = NULL ;
  const char* length = NULL ;
  adv_off_t bytes = 0;

  done = false;
  p = fmt;
  while (!done)
    {
      while (*p && *p++ != '%')
	;
      while (*p)
	{
	  if (*p == '%')
	    {
	      spec = NULL;
	      break;
	    }
	  length = strchr("Lqlh", *p);
	  if (length)
	    p++;
	  spec = strchr("sdif", *p);
	  p++;
	  if (spec)
	    break;
	}
      if (spec)
	{
	  switch (*spec)
	    {
	    case 'd':
	    case 'i':
#ifdef HAVE_ADVSYS_INT64
	      if (length && (*length == 'L' || *length == 'q'))
		bytes += adv_fio_read_int64(file, offset+bytes, va_arg(va, int64*));
	      else
#endif
		bytes += adv_fio_read_int32(file, offset+bytes, va_arg(va, int32*));
	      break;
	    case 'f':
	      if (length && *length == 'l')
		bytes += adv_fio_read_float64(file, offset+bytes, va_arg(va, float64*));
	      else
		bytes += adv_fio_read_float32(file, offset+bytes, va_arg(va, float32*));
	    }
	}
      if (*p == '\0')
	done = true;
    }
  return bytes;
}

int32
adv_fio_fscanf(AdvFile* file, const char* fmt, ...)
{
  va_list va;
  adv_off_t bytes;
  va_start(va, fmt);
  bytes = adv_fio_vfscanf(file, file->current_off, fmt, va);
  va_end(va);
  return bytes;
}

int32
adv_fio_fscanf2(AdvFile* file, adv_off_t offset, const char* fmt, ...)
{
  va_list va;
  adv_off_t bytes;
  va_start(va, fmt);
  bytes = adv_fio_vfscanf(file, offset, fmt, va);
  va_end(va);
  return bytes;
}

