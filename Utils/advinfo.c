/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include <Adv/AdvDocument.h>
#include <Adv/AdvUtil.h>
#include <stdio.h>
#include <unistd.h>

#define  MAX_PROP_STRLEN 1024

int main(int ac, char** av)
{
  AdvDatabox* dbox;
  AdvDocument* doc;
  int i;

  if (ac < 2)
    {
      printf("Usage: %s <Locator>...\n", av[0]);
      printf("$Id: advinfo.c,v 1.1.2.2 2006/02/02 10:05:45 tohyama Exp $\n");
      printf("display property information of <Locator>s.\n");
      printf("Locator must be in format of one of below.\n");
      printf("\n");
      printf("<Locator>:\n");
      printf("   [advdoc:]filename\n");
      printf("   [advdoc:]filename?docid\n");
      printf("   [advdoc:]IOR\n");
      printf("\n");
      return 1;
    }

  adv_dio_init(&ac, &av);

  dbox = adv_dbox_new();
  for (i = 1; i < ac; i++)
    adv_dbox_add(dbox, av[i]);
  
  i = 0;
  while (true)
    {
      int j;
      doc = adv_dbox_open_nth(dbox, i);
      if (!doc)
	break;
      printf("%s\n", adv_dio_get_locator(doc));
      printf("  size: %lld\n", adv_dio_get_size(doc));
      j = 0;
      while (true)
	{
	  char key[MAX_PROP_STRLEN];
	  char val[MAX_PROP_STRLEN];
          char *add_key;
          char *add_val;

          add_key = add_val = "\0";

	  if (adv_dio_get_nth_property(doc, j++, key, MAX_PROP_STRLEN, 
                            val, MAX_PROP_STRLEN)) {
              if ( key[MAX_PROP_STRLEN - 1 ] != '\0' ) {
                 printf( " Warning: property key is too big!!!\n");
                 key[MAX_PROP_STRLEN - 1 ] = '\0' ;
                 add_key = "... ";
              } 
              if ( val[MAX_PROP_STRLEN - 1 ] != '\0' ) {
                 printf( " Warning: property value is too big!!!\n");
                 val[MAX_PROP_STRLEN - 1 ] = '\0' ;
                 add_val = "... ";
              } 
	    printf("  %d: %s%s=%s%s\n", j, key, add_key, val, add_val);
	  } else {
	    break;
          }
	}
      adv_dio_close(doc);
      printf("\n");
      i++;
    }
  adv_dbox_close(dbox);

  a_memory_profile();
  return 0;
}

