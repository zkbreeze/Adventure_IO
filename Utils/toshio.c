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
  AdvDatabox* dbox;
  AdvDocument* doc;
  int num_elems;
  int num_nodes;
  int num_npe;
  adv_off_t off = 0;
  int i, j, n;
  float64 x, y, z;
  int total;
  int num;
  int32 a, b;
  float64 v;

  if (ac <= 1)
    {
      printf("$Id: toshio.c,v 1.1.2.2 2006/02/02 10:05:45 tohyama Exp $\n");
      exit(1);
    }

  dbox = adv_dbox_new();
  adv_dbox_add(dbox, av[1]);


  doc = adv_dbox_find_by_property(dbox, NULL, "content-type", "elements", NULL);
  if (doc)
    {
      off = 0;
      adv_dio_get_property_int32(doc, "items", &num_elems);
      printf("%d\n", num_elems);
      adv_dio_get_property_int32(doc, "nodes_per_element", &num_npe);
      for ( i = 0; i < num_elems; i++)
	{
	  for ( j = 0; j < num_npe; j++)
	    {
	      off += adv_dio_read_int32(doc, off, &n);
	      printf("%d ", n);
	    }
	  printf("\n");
	}
      adv_dio_close(doc);
    }
  else if ( (doc = adv_dbox_find_by_property(dbox, NULL, "content_type", "Element", NULL)) != NULL )
    {
      off = 0;
      adv_dio_get_property_int32(doc, "num_items", &num_elems);
      printf("%d\n", num_elems);
      adv_dio_get_property_int32(doc, "num_nodes_per_element", &num_npe);
      for ( i = 0; i < num_elems; i++)
	{
	  for ( j = 0; j < num_npe; j++)
	    {
	      off += adv_dio_read_int32(doc, off, &n);
	      printf("%d ", n);
	    }
	  printf("\n");
	}
      adv_dio_close(doc);
    }



  doc = adv_dbox_find_by_property(dbox, NULL, "content-type", "nodes", NULL);
  if (doc)
    {
      adv_dio_get_property_int32(doc, "items", &num_nodes);
      printf("%d\n", num_nodes);
      off = 0;
      for ( i = 0; i < num_nodes; i++)
	{
	  off += adv_dio_read_float64(doc, off, &x);
	  off += adv_dio_read_float64(doc, off, &y);
	  off += adv_dio_read_float64(doc, off, &z);
	  printf("%lf %lf %lf\n", x, y, z);
	}
      adv_dio_close(doc);
    }
  else if ( (doc = adv_dbox_find_by_property(dbox, NULL, "content_type", "Node", NULL)) != NULL )
    {
      adv_dio_get_property_int32(doc, "num_items", &num_nodes);
      printf("%d\n", num_nodes);
      off = 0;
      for ( i = 0; i < num_nodes; i++)
	{
	  off += adv_dio_read_float64(doc, off, &x);
	  off += adv_dio_read_float64(doc, off, &y);
	  off += adv_dio_read_float64(doc, off, &z);
	  printf("%lf %lf %lf\n", x, y, z);
	}
      adv_dio_close(doc);
    }

  /* ============================================================ */

  total = 0;
  doc = adv_dbox_find_by_property(dbox, NULL,
				  "content-type", "dirichlet_boundary_conditions",
				  NULL);
  if (doc)
    {
      adv_dio_get_property_int32(doc, "items", &num);
      total += num;
      adv_dio_close(doc);
    }
  if ( ( doc = adv_dbox_find_by_property(dbox, NULL,
				      "content_type", "FEMGenericCondition",
				      "label", "DirichletBoundaryCondition",
				      "fgc_type", "NodeVariable",
				      NULL) ) != NULL )
    {
      do
	{
	  adv_dio_get_property_int32(doc, "num_items", &num);
	  total += num;
	  adv_dio_close(doc);
	} while ( ( doc = adv_dbox_find_by_property(dbox, doc,
						 "content_type", "FEMGenericCondition",
						 "label", "DirichletBoundaryCondition",
						 "fgc_type", "NodeVariable",
						 NULL) ) != NULL );
    }
  printf("%d\n", total);
  doc = adv_dbox_find_by_property(dbox, NULL,
				  "content-type", "dirichlet_boundary_conditions",
				  NULL);
  if (doc)
    {
      adv_dio_get_property_int32(doc, "items", &num);
      off = 0;
      for ( i = 0; i < num; i++)
	{
	  off += adv_dio_read_int32(doc, off, &a);
	  off += adv_dio_read_int32(doc, off, &b);
	  off += adv_dio_read_float64(doc, off, &v);
	  printf("%d %d %lf\n", a, b, v);
	}
      adv_dio_close(doc);
    }
  if ( ( doc = adv_dbox_find_by_property(dbox, NULL,
				      "content_type", "FEMGenericCondition",
				      "label", "DirichletBoundaryCondition",
				      "fgc_type", "NodeVariable",
				      NULL) ) != NULL )
    {
      do
	{
	  adv_dio_get_property_int32(doc, "num_items", &num);
	  off = 0;
	  for ( i = 0; i < num; i++)
	    {
	      off += adv_dio_read_int32(doc, off, &a);
	      off += adv_dio_read_int32(doc, off, &b);
	      off += adv_dio_read_float64(doc, off, &v);
	      printf("%d %d %lf\n", a, b, v);
	    }
	  adv_dio_close(doc);
	} while ( ( doc = adv_dbox_find_by_property(dbox, doc,
						 "content_type", "FEMGenericCondition",
						 "label", "DirichletBoundaryCondition",
						 "fgc_type", "NodeVariable",
						 NULL) ) != NULL );
    }

  /* ============================================================ */

  total = 0;

  doc = adv_dbox_find_by_property(dbox, NULL,
				  "content-type", "neumann_boundary_conditions",
				  NULL);
  if (doc)
    {
      adv_dio_get_property_int32(doc, "items", &num);
      total += num;
      adv_dio_close(doc);
    }
  if ( ( doc = adv_dbox_find_by_property(dbox, NULL,
				      "content_type", "FEMGenericCondition",
				      "label", "NeumannBoundaryCondition",
				      "fgc_type", "NodeVariable",
				      NULL) ) != NULL )
    {
      do
	{
	  adv_dio_get_property_int32(doc, "num_items", &num);
	  total += num;
	  adv_dio_close(doc);
	} while ( ( doc = adv_dbox_find_by_property(dbox, doc,
						 "content_type", "FEMGenericCondition",
						 "label", "NeumannBoundaryCondition",
						 "fgc_type", "NodeVariable",
						 NULL) ) != NULL );
    }

  printf("%d\n", total);

  doc = adv_dbox_find_by_property(dbox, NULL,
				  "content-type", "neumann_boundary_conditions",
				  NULL);
  if (doc)
    {
      adv_dio_get_property_int32(doc, "items", &num);
      off = 0;
      for ( i = 0; i < num; i++)
	{
	  off += adv_dio_read_int32(doc, off, &a);
	  off += adv_dio_read_int32(doc, off, &b);
	  off += adv_dio_read_float64(doc, off, &v);
	  printf("%d %d %lf\n", a, b, v);
	}
      adv_dio_close(doc);
    }
  if ( ( doc = adv_dbox_find_by_property(dbox, NULL,
				      "content_type", "FEMGenericCondition",
				      "label", "NeumannBoundaryCondition",
				      "fgc_type", "NodeVariable",
				      NULL) ) != NULL )
    {
      do
	{
	  adv_dio_get_property_int32(doc, "num_items", &num);
	  off = 0;
	  for ( i = 0; i < num; i++)
	    {
	      off += adv_dio_read_int32(doc, off, &a);
	      off += adv_dio_read_int32(doc, off, &b);
	      off += adv_dio_read_float64(doc, off, &v);
	      printf("%d %d %lf\n", a, b, v);
	    }
	  adv_dio_close(doc);
	} while ( ( doc = adv_dbox_find_by_property(dbox, doc,
						 "content_type", "FEMGenericCondition",
						 "label", "NeumannBoundaryCondition",
						 "fgc_type", "NodeVariable",
						 NULL) ) != NULL );
    }

  /* dump young */

  /* dump poisson */

  adv_dbox_close(dbox);
  return 0;
}
