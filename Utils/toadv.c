/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include <Adv/AdvDocument.h>
#include <Adv/AdvUtil.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#ifdef USE_ZLIB

#ifdef feof
#undef feof
#endif

#include <zlib.h>
#define FILEP gzFile
#define fopen gzopen
#define fclose gzclose
#define feof gzeof
#define rewind gzrewind
#define fgets(a,b,c) gzgets(c,a,b)
#else
#define FILEP FILE*
#endif

enum FileType {TYPE_SHIOYA, TYPE_FEM, TYPE_CONTROL, TYPE_VALUES, TYPE_TEDDY };

struct
{
  bool verbose;
  bool fortran;
  bool onlynodes;
  bool old;
} flags;


void
message(const char* fmt, ...)
{
  va_list va;
  if (flags.verbose)
    {
      va_start(va, fmt);
      vprintf(fmt, va);
      va_end(va);
    }
}

int
count_number(const char* str)
{
  const char* p;
  const char* q;
  int num = 0;
  p = NULL;
  q = str;
  while (true)
    {
      p = q;
      strtod(p, (char**)&q);
      if (p == q)
	break;
      else
	num++;
    }
  return num;
}

int check_type(FILEP fp)
{
  char buf[2][1024];
  int nums[2];
  int i;

  rewind(fp);
  fgets(buf[0], 1024, fp);
  fgets(buf[1], 1024, fp);

  if (buf[0][0] == 'v' && buf[0][1] == ' ')
    return TYPE_TEDDY;

  if (isalpha(buf[0][0]))
    return TYPE_CONTROL;

  for ( i = 0; i < 2; i++)
    nums[i] = count_number(buf[i]);

  if (nums[0] == 1 && nums[1] == 1)
    return TYPE_VALUES;
  else if (nums[0] == 1 && nums[1] >= 3)
    return TYPE_SHIOYA;
  else if (nums[0] == 2 && (nums[1] == 2 || nums[1] == 3))
    return TYPE_FEM;
  else
    return TYPE_FEM;
}

const char*
guess_element_type(int dim, int npe)
{
  const char* type = "unknown";
  if (dim == 3)
    {
      switch (npe)
	{
	case 3:
	  type = "3DLinearTriangle";
	  break;
	case 4:
	  type = "3DLinearTetrahedron";
	  break;
	case 8:
	  type = "3DLinearHexahedron";
	  break;
	case 10:
	  type = "3DQuadraticTetrahedron";
	  break;
	case 20:
	  type = "3DQuadraticHexahedron";
	  break;
	}
    }
  else if (dim == 2)
    {
    }
  return type;
}

const char*
guess_element_format(int dim, int npe)
{
  static char buf[256];
  int i;

  buf[0] = '\0';
  for ( i = 0; i < npe; i++)
    strcat(buf, "i4");
  return buf;
}

void
conv_shioya(FILEP in, AdvDocFile* out)
{
  char line[1024];
  adv_off_t off = 0;
  int npe = -1;
  int dim;
  int offset, i, j ;
  int num_nodes;
  AdvDocument* doc = NULL;
  int num_nbcs;
  int num_elems;
  int num_dbcs;
  double val;
  char *p;
  char* q;


  rewind(in);

  offset = flags.fortran ? 1 : 0;
  message("offset %d\n", offset);

  if (!flags.onlynodes)
    {
      fgets(line, sizeof(line), in);
      num_elems = atoi(line);
      message("elements %d", num_elems);
      doc = adv_dio_create(out, adv_dio_make_documentid("Elements"));
      if (flags.old)
	{
	  adv_dio_set_property(doc, "content-type", "elements");
	  adv_dio_set_property_int32(doc, "items", num_elems);
	}
      adv_dio_set_property(doc, "content_type", "Element");
      adv_dio_set_property_int32(doc, "num_items", num_elems);
      off = 0;
      npe = -1;
      for ( i = 0; i < num_elems; i++)
	{
	  fgets(line, sizeof(line), in);
	  if (npe == -1)
	    {
	      npe = count_number(line);
	      message("(%d nodes)\n", npe);
	      if (flags.old)
		adv_dio_set_property_int32(doc, "nodes_per_element", npe);
	      adv_dio_set_property_int32(doc, "num_nodes_per_element", npe);
	    }
	  p = line;
	  for ( j = 0; j < npe; j++)
	    off += adv_dio_write_int32(doc, off, strtol(p, &p, 10)-offset);
	}
    }

  fgets(line, sizeof(line), in);
  num_nodes = atoi(line);
  message("nodes %d", num_nodes);
  off = 0;
  dim = -1;
  for ( i = 0; i < num_nodes; i++)
    {
      fgets(line, sizeof(line), in);
      if (dim == -1)
	{
	  dim = count_number(line);
	  message("(%d axis, %s)\n", dim, guess_element_type(dim, npe));
	  adv_dio_set_property_int32(doc, "dimension", dim);
	  adv_dio_set_property(doc, "element_type", guess_element_type(dim, npe));
	  adv_dio_set_property(doc, "format", guess_element_format(dim, npe));
	  adv_dio_close(doc);
	  doc = adv_dio_create(out, adv_dio_make_documentid("Nodes"));
	  if (flags.old)
	    {
	      adv_dio_set_property(doc, "content-type", "nodes");
	      adv_dio_set_property_int32(doc, "num_items", num_nodes);
	    }
	  adv_dio_set_property(doc, "content_type", "Node");
	  adv_dio_set_property_int32(doc, "num_items", num_nodes);
	  adv_dio_set_property_int32(doc, "dimension", dim);
	  if (dim == 2)
	    adv_dio_set_property(doc, "format", "f8f8");
	  else
	    adv_dio_set_property(doc, "format", "f8f8f8");
	}
      p = line;
      for ( j = 0; j < dim; j++)
	off += adv_dio_write_float64(doc, off, strtod(p, &p));
    }
  adv_dio_close(doc);

  if (!flags.onlynodes)
    {
      fgets(line, sizeof(line), in);
      num_dbcs = atoi(line);
      if (num_dbcs > 0)
	{
	  message("dbcs %d\n", num_dbcs);
	  doc = adv_dio_create(out, adv_dio_make_documentid("DirichletBCs"));
	  if (flags.old)
	    {
	      adv_dio_set_property(doc, "content-type", "dirichlet_boundary_conditions");
	      adv_dio_set_property_int32(doc, "items", num_dbcs);
	    }
	  adv_dio_set_property(doc, "content_type", "FEGenericAttribute");
	  adv_dio_set_property_int32(doc, "num_items", num_dbcs);
	  adv_dio_set_property(doc, "fega_type", "NodeVariable");
	  adv_dio_set_property(doc, "label", "ForceDisplacement");
	  adv_dio_set_property(doc, "format", "i4f8");
	  adv_dio_set_property(doc, "dd_option", "");
	  adv_dio_set_property(doc, "index_byte", "4");
	  off = 0;
	  for ( i = 0; i < num_dbcs; i++)
	    {
	      fgets(line, sizeof(line), in);
	      p = line;
	      off += adv_dio_write_int32(doc, off, strtol(p, &p, 10));
	      off += adv_dio_write_int32(doc, off, strtol(p, &p, 10));
	      off += adv_dio_write_float64(doc, off, strtod(p, &p));
	    }
	  adv_dio_close(doc);
	}

      fgets(line, sizeof(line), in);
      num_nbcs = atoi(line);
      if (num_nbcs > 0)
	{
	  message("nbcs %d\n", num_nbcs);
	  doc = adv_dio_create(out, adv_dio_make_documentid("NeumannBCs"));
	  if (flags.old)
	    {
	      adv_dio_set_property(doc, "content-type", "neumann_boundary_conditions");
	      adv_dio_set_property_int32(doc, "items", num_nbcs);
	    }
	  adv_dio_set_property(doc, "content_type", "FEGenericAttribute");
	  adv_dio_set_property_int32(doc, "num_items", num_nbcs);
	  adv_dio_set_property(doc, "fega_type", "NodeVariable");
	  adv_dio_set_property(doc, "label", "Load");
	  adv_dio_set_property(doc, "format", "i4f8");
	  adv_dio_set_property(doc, "dd_option", "");
	  adv_dio_set_property(doc, "index_byte", "4");
	  off = 0;
	  for ( i = 0; i < num_nbcs; i++)
	    {
	      fgets(line, sizeof(line), in);
	      p = line;
	      off += adv_dio_write_int32(doc, off, strtol(p, &p, 10));
	      off += adv_dio_write_int32(doc, off, strtol(p, &p, 10));
	      off += adv_dio_write_float64(doc, off, strtod(p, &p));
	    }
	  adv_dio_close(doc);
	}

      fgets(line, sizeof(line), in);
      p = line;
      val = strtod(p, &q);
      if (q != p)
	{
	  message("young: %lf\n", val);
	  doc = adv_dio_create(out, adv_dio_make_documentid("Attr"));
	  adv_dio_set_property(doc, "content_type", "FEGenericAttribute");
	  adv_dio_set_property_int32(doc, "num_items", 1);
	  adv_dio_set_property(doc, "fega_type", "AllElementConstant");
	  adv_dio_set_property(doc, "label", "YoungModule");
	  adv_dio_set_property(doc, "format", "f8");
	  adv_dio_set_property(doc, "dd_option", "");
	  adv_dio_set_property(doc, "index_byte", "4");
	  adv_dio_set_property_float64(doc, "young_module", val);
	  adv_dio_write_float64(doc, 0, val);
	  adv_dio_close(doc);
	}

      fgets(line, sizeof(line), in);
      p = line;
      val = strtod(p, &p);
      if (q != p)
	{
	  message("poisson: %lf\n", val);
	  doc = adv_dio_create(out, adv_dio_make_documentid("Attr"));
	  adv_dio_set_property(doc, "content_type", "FEGenericAttribute");
	  adv_dio_set_property_int32(doc, "num_items", 1);
	  adv_dio_set_property(doc, "fega_type", "AllElementConstant");
	  adv_dio_set_property(doc, "label", "PoissonRatio");
	  adv_dio_set_property(doc, "format", "f8");
	  adv_dio_set_property(doc, "dd_option", "");
	  adv_dio_set_property(doc, "index_byte", "4");
	  adv_dio_set_property_float64(doc, "poisson_ratio", val);
	  adv_dio_write_float64(doc, 0, val);
	  adv_dio_close(doc);
	}
    }
}

void
conv_fem(FILEP in, AdvDocFile* out)
{
  char line[1024];
  adv_off_t off = 0;
  AdvDocument* doc;
  int npe;
  int dim;
  int num_nodes;
  int num_elems;
  char* p;
  int i, j;
  int offset ;

  rewind(in);
  fgets(line, sizeof(line), in);

  p = line;
  num_nodes = strtol(p, &p, 10);
  num_elems = strtol(p, &p, 10);
  message("nodes %d", num_nodes);
  doc = adv_dio_create(out, adv_dio_make_documentid("Nodes"));
  if (flags.old)
    {
      adv_dio_set_property(doc, "content-type", "nodes");
      adv_dio_set_property_int32(doc, "num_items", num_nodes);
    }
  adv_dio_set_property(doc, "content_type", "Node");
  adv_dio_set_property_int32(doc, "num_items", num_nodes);
  off = 0;
  dim = -1;
  for ( i = 0; i < num_nodes; i++)
    {
      fgets(line, sizeof(line), in);
      if (dim == -1)
	{
	  dim = count_number(line);
	  message("(%d axis)\n", dim);
	  adv_dio_set_property_int32(doc, "dimension", dim);
	  if (dim == 2)
	    adv_dio_set_property(doc, "format", "f8f8");
	  else if (dim == 3)
	    adv_dio_set_property(doc, "format", "f8f8f8");
	}
      p = line;
      for ( j = 0; j < dim; j++)
	off += adv_dio_write_float64(doc, off, strtod(p, &p));
    }
  adv_dio_close(doc);

  offset = flags.fortran ? 1 : 0;
  message("offset %d\n", offset);

  if (!flags.onlynodes)
    {
      message("elements %d", num_elems);
      doc = adv_dio_create(out, adv_dio_make_documentid("Elements"));
      off = 0;
      npe = -1;
      for ( i = 0; i < num_elems; i++)
	{
	  fgets(line, sizeof(line), in);
	  if (npe == -1)
	    {
	      npe = count_number(line);
	      message("(%d nodes, %s)\n", npe, guess_element_type(dim, npe));
	      if (flags.old)
		{
		  adv_dio_set_property(doc, "content-type", "elements");
		  adv_dio_set_property_int32(doc, "items", num_elems);
		  adv_dio_set_property_int32(doc, "nodes_per_element", npe);
		}
	      adv_dio_set_property(doc, "content_type", "Element");
	      adv_dio_set_property_int32(doc, "num_items", num_elems);
	      adv_dio_set_property_int32(doc, "num_nodes_per_element", npe);
	      adv_dio_set_property_int32(doc, "dimension", dim);
	      adv_dio_set_property(doc, "element_type", guess_element_type(dim, npe));
	      adv_dio_set_property(doc, "format", guess_element_format(dim, npe));
	    }
	  p = line;
	  for ( j = 0; j < npe; j++)
	    off += adv_dio_write_int32(doc, off, strtol(p, &p, 10)-offset);
	}
      adv_dio_close(doc);
    }
}

void
conv_control(FILEP in, AdvDocFile* out)
{
  char line[1024];
  AdvDocument* doc;
  char* p;
  char* key;
  char* val;

  rewind(in);
  fgets(line, sizeof(line), in);

  doc = adv_dio_create(out, adv_dio_make_documentid("Control"));
  do
    {
      p = line;
      while (*p && isspace(*p))
	p++;
      strcpy(line, p);
      p = line+strlen(line)-1;
      if (p >= line && isspace(*p))
	p--;
      *(p+1) = '\0';
      if (line[0] == '-')
	{
	  adv_dio_close(doc);
	  doc = adv_dio_create(out, adv_dio_make_documentid("Control"));
	}
      key = line;
      val = strchr(line, '=');
      if (val)
	{
	  *val = '\0';
	  val++;
	  adv_dio_set_property(doc, key, val);
	}
      fgets(line, sizeof(line), in);
    } while (!feof(in));
  adv_dio_close(doc);
}

void
conv_values(FILEP in, AdvDocFile* out)
{
  char line[1024];
  AdvDocument* doc;
  int items = 0;
  adv_off_t off = 0;
  float64 val ;

  rewind(in);
  doc = adv_dio_create(out, adv_dio_make_documentid("Values"));
  adv_dio_set_property(doc, "content_type", "FEGenericAttribute");
  adv_dio_set_property(doc, "label", "ScalarOnNode");
  adv_dio_set_property(doc, "fega_type", "AllNodeVariable");
  adv_dio_set_property(doc, "format", "f8");
  adv_dio_set_property(doc, "index_byte", "4");
  adv_dio_set_property(doc, "dd_option", "");

  while (!feof(in))
    {
      fgets(line, sizeof(line), in);
      if (strpbrk(line, "0123456789") == NULL)
	continue;
      val = strtod(line, NULL);
      off += adv_dio_write_float64(doc, off, val);
      items++;
    }
  adv_dio_set_property_int32(doc, "num_items", items);
  printf("items %d\n", items);
  adv_dio_close(doc);
}

void
conv_teddy(FILEP in, AdvDocFile* out)
{
  char line[1024];
  adv_off_t off = 0;
  AdvDocument* doc;

  int npe;
  int dim;

  int num_nodes;
  int num_elems;
  char* p;
  int i,j;
  int offset;

  p = line;
  num_nodes = 0;
  num_elems = 0;
  dim = 3;
  npe = 3;

  rewind(in);

  while (!feof(in))
    {
      fgets(line, sizeof(line), in);
      if (line[0] == 'v')
	num_nodes++;
      else if (line[0] == 'f')
	num_elems++;
    }
      
  rewind(in);

  doc = adv_dio_create(out, adv_dio_make_documentid("Nodes"));

  message("nodes %d", num_nodes);
  message("(%d axis)\n", dim);

  adv_dio_set_property(doc, "content_type", "Node");
  adv_dio_set_property_int32(doc, "num_items", num_nodes);
  adv_dio_set_property_int32(doc, "dimension", dim);

  off = 0;
  for ( i = 0; i < num_nodes; i++)
    {
      fgets(line, sizeof(line), in);
      if (line[0] == 'v')
	{
	  p = line+2;
	  for ( j = 0; j < dim; j++)
	    off += adv_dio_write_float64(doc, off, strtod(p, &p));
	}
    }
  adv_dio_close(doc);

  offset = 1;

  doc = adv_dio_create(out, adv_dio_make_documentid("Elements"));

  message("elements %d", num_elems);
  message("(%d nodes, %s)\n", npe, guess_element_type(dim, npe));

  adv_dio_set_property(doc, "content_type", "Element");
  adv_dio_set_property_int32(doc, "num_items", num_elems);
  adv_dio_set_property_int32(doc, "num_nodes_per_element", npe);
  adv_dio_set_property(doc, "element_type", guess_element_type(dim, npe));

  off = 0;
  for ( i = 0; i < num_elems; i++)
    {
      fgets(line, sizeof(line), in);
      if (line[0] == 'f')
	{
	  p = line+2;
	  for ( j = 0; j < npe; j++)
	    off += adv_dio_write_int32(doc, off, strtol(p, &p, 10)-offset);
	}
    }
  adv_dio_close(doc);
}

int
main(int ac, char** av)
{
  int i;
  const char* infilename ;
  const char* outfilename ;
  int unknown_opts = 0;
  FILEP inf ;
  AdvDocFile* outf ;

  adv_dio_init(&ac, &av);
  flags.verbose = adv_opt_get_flag(&ac, &av, "v", "verbose");
  flags.fortran = adv_opt_get_flag(&ac, &av, "f", "fortran");
  flags.onlynodes = adv_opt_get_flag(&ac, &av, "n", "only-nodes");
  flags.old = adv_opt_get_flag(&ac, &av, "o", "old");

  for ( i = 1; i < ac; i++)
    {
      if (av[i][0] == '-')
	{
	  printf("Unknown option: %s\n", av[i]);
	  unknown_opts++;
	}
    }
  if (unknown_opts > 0)
    return 1;
  if (ac < 3)
    {
      printf("%s [OPTIONS] <INFILE> <OUTFILE>\n", av[0]);
      printf("$Id: toadv.c,v 1.1.2.2 2006/02/02 10:05:45 tohyama Exp $\n");
      printf("  -v, --verbose  verbose\n");
      printf("  -f, --fortran  assume the minimum of node numbers in element conectivity is 1 (not 0)\n");
      printf("  -v, --verbose   verbose\n");
      return 1;
    }
  infilename = av[1];
  outfilename = av[2];
  
  inf = fopen(infilename, "r");
  if (!inf)
    {
      printf("cannot open input file: %s\n", infilename);
      return 1;
    }
  outf = adv_dio_file_open(outfilename, "c");
  if (!outf)
    {
      printf("cannot open output file: %s\n", outfilename);
      return 1;
    }
  switch (check_type(inf))
    {
    case TYPE_SHIOYA:
      message("type: shioya\n");
      conv_shioya(inf, outf);
      break;
    case TYPE_FEM:
      message("fem\n");
      conv_fem(inf, outf);
      break;
    case TYPE_CONTROL:
      message("control\n");
      conv_control(inf, outf);
      break;
    case TYPE_VALUES:
      message("values\n");
      conv_values(inf, outf);
      break;
    case TYPE_TEDDY:
      message("teddy object\n");
      conv_teddy(inf, outf);
      break;
    }
  fclose(inf);
  adv_dio_file_close(outf);

  return 0;
}
