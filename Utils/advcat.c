/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include <Adv/AdvDocument.h>
#include <Adv/AdvUtil.h>
#include <stdio.h>
#include <stdarg.h>

void
usage(const char* cmd)
{
  printf("Usage: %s <Input>... <Outfile> [-o <Output>]\n", cmd);
  printf("$Id: advcat.c,v 1.3.2.1 2006/02/02 10:05:45 tohyama Exp $\n");
  printf("Dump all input to single output\n");
  printf("The last argument used as output unless output is not specified explicitly with `-o'.\n");
  printf("\n");
  printf("[Ex.]\n");
  printf(" %s file_a file_b file_c\n", cmd);
  printf("    Dumps file_a, file_b into file_c\n");
  printf(" %s -o file_a file_b file_c\n", cmd);
  printf("    Dumps file_b, file_c into file_a\n");
  printf("\n");
}

void
adios(int code, const char* msg, ...)
{
  va_list va;
  va_start(va, msg);
  vfprintf(stderr, msg, va);
  va_end(va);
  exit(code);
}

int
main(int ac, char** av)
{
  AdvDocument* srcdoc;
  AdvDocFile* srcfile;
  AdvDocFile* dfile;
  char* outfname = NULL;
  int i, n, docs;
  
  adv_dio_init(&ac, &av);
  adv_opt_get_string(&ac, &av, "o", "output", &outfname);
  if (outfname == NULL)
    {
      outfname = av[ac-1];
      ac--;
    }
  if (ac < 2)
    {
      usage(av[0]);
      exit(1);
    }
  printf("Output: %s\n", outfname);

  dfile = adv_dio_file_open(outfname, "c");
  if (!dfile)
    adios(1, "Cannot open output\n");

  docs = 0;
  for (i = 1; i < ac; i++)
    {
      if ( (srcdoc = adv_dio_open_by_locator(av[i])) != NULL )
	{
	  printf("Dumping: %s\n", adv_dio_get_documentid(srcdoc));
	  adv_dio_copy_to_file(dfile, srcdoc);
	  adv_dio_close(srcdoc);
	  docs++;
	}
      else if ( (srcfile = adv_dio_file_open(av[i], "r")) != NULL )
	{
	  n = 0;
	  while ( (srcdoc = adv_dio_open_nth(srcfile, n++)) != NULL )
	    {
	      printf("Dumping: %s\n", adv_dio_get_documentid(srcdoc));
	      adv_dio_copy_to_file(dfile, srcdoc);
	      adv_dio_close(srcdoc);
	      docs++;
	    }
	  adv_dio_file_close(srcfile);
	}
      else
	printf("Cannot open %s (ignored)\n", av[i]);
    }

  adv_dio_file_close(dfile);

  printf("\nTotal %d document%s dumped.\n", docs, docs >= 2 ? "s" : "");
  return 0;
}
