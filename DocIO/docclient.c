/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include <stddef.h>
#include <Adv/AdvTypes.h>
#include "AdvIDocument.h"
#include <orb/orbit.h>

int
main(int ac, char** av)
{
  CORBA_ORB orb;
  CORBA_Environment env;
  AdvIDocFile* dfile;
  AdvIDocument* doc;
  int32 size;
  char buf[100];

  adv_idio_init(&ac, &av);
  dfile = adv_idio_file_open(av[1], "r");
  printf("%s\n", adv_idio_file_get_locator(dfile));
  doc = adv_idio_create(dfile, "aaa");
  adv_idio_set_property(doc, "Hello", "World");
  adv_idio_set_property_int32(doc, "Good", 60);
  adv_idio_set_property_float64(doc, "Heavy", 0.103);
  adv_idio_close(doc);
  adv_idio_file_close(dfile);

  dfile = adv_idio_file_open(av[1], "r");
  doc = adv_idio_open_nth(dfile, 0);
  size = adv_idio_get_size(doc);
  printf("size %d\n", size);
  adv_idio_read(doc, 0, size, buf);
  printf(buf);
  adv_idio_write(doc, 0, 10, "ggghhhiiij");
  adv_idio_close(doc);
  adv_idio_file_close(dfile);

  return 0;
}
