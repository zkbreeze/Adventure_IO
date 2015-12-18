/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include "AdvDocument.h"
#include <Adv/AdvUtil.h>
#include <stdio.h>
#include <unistd.h>

int
main(int ac, char** av)
{
  AdvFDocFile* dfile;
  AdvFDocument* doc;
  adv_off_t off = 0;
  int i;
  int items;
  const char* testfile = "testadvio.adv";

  /*--- write ---*/

  printf("writing...\n");
  dfile = adv_fdio_file_open(testfile, "c");

  printf("writing document1\n");
  doc = adv_fdio_create(dfile, adv_dio_make_documentid("document_prop"));
  adv_fdio_set_property         (doc, "prop1", "string");
  adv_fdio_set_property_int32   (doc, "prop2", 111111);
  adv_fdio_set_property_float64 (doc, "prop3", 1.1111);
  adv_fdio_close(doc);

  /* write int32 */
  printf("writing document2\n");
  doc = adv_fdio_create(dfile, adv_dio_make_documentid("document_int"));
  items = 100;
  adv_fdio_set_property_int32(doc, "items", items);
  off = 0;
  for (i = 0; i < items; i++)
    off += adv_fdio_write_int32(doc, off, i);
  adv_fdio_close(doc);

  adv_fdio_file_close(dfile);

  /*--- append ---*/

  dfile = adv_fdio_file_open(testfile, "a");

  printf("appending...\n");

  /* write float64 */
  printf("writing document3\n");
  doc = adv_fdio_create(dfile, adv_dio_make_documentid("document_float"));
  items = 50;
  adv_fdio_set_property_int32(doc, "items", items);
  off = 0;
  for (i = 0; i < items; i++)
    off += adv_fdio_write_float64(doc, off, (float64)i/10);
  adv_fdio_close(doc);

  for (i = 0; i < 200; i++)
    {
      doc = adv_fdio_create(dfile, adv_dio_make_documentid("document"));
      adv_fdio_close(doc);
    }

  adv_fdio_file_close(dfile);

  /*--- read ---*/

  printf("reading...\n");
  dfile = adv_fdio_file_open(testfile, "r");
  i = 0;
  while (true)
    {
      int j;
      doc = adv_fdio_open_nth(dfile, i);
      if (!doc)
	break;
      printf("%d: %s\n", i+1, adv_fdio_get_documentid(doc));
      j = 0;
      while (true)
	{
	  char key[128];
	  char val[128];
	  if (adv_fdio_get_nth_property(doc, j++, key, 128, val, 128))
	    printf("  %d: %s=%s\n", j, key, val);
	  else
	    break;
	}
      i++;
    }

  /*--- databox ---*/
  
  {
    AdvDatabox* adb = adv_dbox_new();
    AdvDocument* doc;
    int j;
    printf("databox...\n");
    adv_dbox_add(adb, testfile);
    doc = adv_dbox_find_by_property(adb, NULL, "prop1", "string", NULL);
    j = 0;
    while (true)
      {
	char key[128];
	char val[128];
	if (adv_dio_get_nth_property(doc, j++, key, 128, val, 128))
	  printf("  %d: %s=%s\n", j, key, val);
	else
	  break;
      }
    adv_dio_close(doc);
    adv_dbox_close(adb);
  }

  /*--- format ---*/
  
  {
    const char* format = "i1i2i4f4f8";
    octet* buf;
    int8 i8;
    int16 i16;
    int32 i32;
    float32 f32;
    float64 f64;
    printf("---format test---\n");
    printf("format string: %s\n", format);
    printf("need buffer size: %d\n", adv_format_get_size(format));
    buf = (octet*)malloc(adv_format_get_size(format));
    adv_format_pack(buf, format, 1, 2, 3, 4.0, 5.0);
    adv_format_unpack(buf, format, &i8, &i16, &i32, &f32, &f64);
    printf("pack test: %d %d %d %f %f\n", (int32)i8, (int32)i16, (int32)i32, (float32)f32, (float64)f64);
  }

  adv_fdio_file_close(dfile);

  a_memory_profile();
  return 0;
}
