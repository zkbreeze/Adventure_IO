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
  char* msg = "Hello, World.coca-cola pepsi-cola abcdefghijklmnopqrstuvwxxyz\n";

  adv_idio_init(&ac, &av);
  dfile = adv_idio_file_open(NULL, NULL);
  printf("%s\n", adv_idio_file_get_locator(dfile));
  doc = adv_idio_create(dfile, "ccc");
  adv_idio_write(doc, 0, strlen(msg)+1, msg);
  adv_idio_close(doc);

  adv_idio_run();
  return 0;
}
