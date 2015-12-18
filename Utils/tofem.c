/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include <Adv/AdvDocument.h>
#include <stdlib.h>
#include <stdio.h>

int main(int ac, char** av)
{
  int i,j;
  AdvDatabox* dbox;
  AdvDocument* edoc;
  AdvDocument* ndoc;
  int num_elems;
  int num_nodes;
  adv_off_t off = 0;
  float64 x, y, z;
  int num_npe;
  int n;

  if (ac <= 1)
    {
      printf("$Id: tofem.c,v 1.1.2.2 2006/02/02 10:05:45 tohyama Exp $\n");
      exit(1);
    }

  dbox = adv_dbox_new();
  adv_dbox_add(dbox, av[1]);

  edoc = adv_dbox_find_by_property(dbox, NULL, "content-type", "elements", NULL);
  ndoc = adv_dbox_find_by_property(dbox, NULL, "content-type", "nodes", NULL);

  if (edoc && ndoc)
    {
      adv_dio_get_property_int32(edoc, "items", &num_elems);
      adv_dio_get_property_int32(ndoc, "items", &num_nodes);
      printf("%d %d\n", num_nodes, num_elems);

      off = 0;
      for ( i = 0; i < num_nodes; i++)
	{
	  off += adv_dio_read_float64(ndoc, off, &x);
	  off += adv_dio_read_float64(ndoc, off, &y);
	  off += adv_dio_read_float64(ndoc, off, &z);
	  printf("%lf %lf %lf\n", x, y, z);
	}

      off = 0;
      adv_dio_get_property_int32(edoc, "nodes_per_element", &num_npe);
      for ( i = 0; i < num_elems; i++)
	{
	  for ( j = 0; j < num_npe; j++)
	    {
	      off += adv_dio_read_int32(edoc, off, &n);
	      printf("%d ", n+1);
	    }
	  printf("\n");
	}
      adv_dio_close(ndoc);
      adv_dio_close(edoc);
    }
  else
    {
      edoc = adv_dbox_find_by_property(dbox, NULL, "content_type", "Element", NULL);
      ndoc = adv_dbox_find_by_property(dbox, NULL, "content_type", "Node", NULL);
      if (edoc && ndoc)
	{
	  adv_dio_get_property_int32(edoc, "num_items", &num_elems);
	  adv_dio_get_property_int32(ndoc, "num_items", &num_nodes);
	  printf("%d %d\n", num_nodes, num_elems);

	  off = 0;
	  for ( i = 0; i < num_nodes; i++)
	    {
	      off += adv_dio_read_float64(ndoc, off, &x);
	      off += adv_dio_read_float64(ndoc, off, &y);
	      off += adv_dio_read_float64(ndoc, off, &z);
	      printf("%lf %lf %lf\n", x, y, z);
	    }

	  off = 0;
	  adv_dio_get_property_int32(edoc, "num_nodes_per_element", &num_npe);
	  for ( i = 0; i < num_elems; i++)
	    {
	      for ( j = 0; j < num_npe; j++)
		{
		  off += adv_dio_read_int32(edoc, off, &n);
		  printf("%d ", n+1);
		}
	      printf("\n");
	    }
	  adv_dio_close(ndoc);
	  adv_dio_close(edoc);
	}
    }      
  adv_dbox_close(dbox);

  return 0;
}
