/***********************************************************
Copyright (C) 2000,2001,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <Adv/AdvFileIO.h>
#include "mesh.h"

Mesh::Mesh()
  : ref_count(1)
{
  num_nodes = 0;
  nodes = NULL;

  num_elems = 0;
  num_nodes_per_elem = 0;
  elems = NULL;

  num_surfaces = 0;
  num_innfaces = 0;
  num_nodes_per_face = 0;
  faces = NULL;
}

Mesh::~Mesh()
{
  delete nodes;
  delete elems;
  delete faces;

  // !!!FIXME!!! should cleanup BC here.
}

Mesh*
Mesh::Create()
{
  return new Mesh;
}

void
Mesh::Ref()
{
  ref_count++;
}

void
Mesh::Unref()
{
  if (--ref_count == 0)
    delete this;
}

void
Mesh::DeleteNodes()
{
  delete nodes;
  nodes = NULL;
  num_nodes = 0;
  message("node deleted\n");
}

void
Mesh::DeleteElems()
{
  delete elems;
  elems = NULL;
  num_elems = 0;
}

void
Mesh::DeleteFaces()
{
  delete faces;
  faces = NULL;
  num_surfaces = 0;
  num_innfaces = 0;
}

void
Mesh::ReadData(AdvDatabox* dbox)
{
  AdvDocument* doc;

  doc = adv_dbox_find_by_property(dbox, NULL, "content_type", "Node", NULL);
  if (doc)
    ReadNodes(doc);

  doc = adv_dbox_find_by_property(dbox, NULL, "content_type", "Element", NULL);
  if (doc)
    ReadElems(doc);

  doc = adv_dbox_find_by_property(dbox, NULL, "content_type", "dirichlet_boundary_conditions", NULL);
  if (doc)
    ReadBCs(doc);

  doc = adv_dbox_find_by_property(dbox, NULL, "content_type", "neumann_boundary_conditions", NULL);
  if (doc)
    ReadBCs(doc);
}

bool
Mesh::ReadElems(AdvDatabox* dbox)
{
  AdvDocument* doc;

  doc = adv_dbox_find_by_property(dbox, NULL, "content_type", "Element", NULL);
  if (doc)
    {
      ReadElems(doc);
      return true;
    }
  else
    return false;
}

bool
Mesh::ReadNodes(AdvDatabox* dbox)
{
  AdvDocument* doc;

  doc = adv_dbox_find_by_property(dbox, NULL, "content_type", "Node", NULL);
  if (doc)
    {
      ReadNodes(doc);
      return true;
    }
  else
    return false;
}

bool
Mesh::ReadNodes(AdvDocument* doc)
{
  int32 num;
  int32 dim;
  float32 d_x, d_y, d_z, d;
  
  num_nodes = 0;
  delete nodes;
  nodes = NULL;

  adv_dio_get_property_int32(doc, "num_items", &num);
  adv_dio_get_property_int32(doc, "dimension", &dim);
  if (dim != 3)
    return false;
  else
    {
      num_nodes = num;
      nodes = new Node[num];
    }
  adv_off_t off = 0;
  for (int i = 0; i < num; i++)
    {
      float64 x, y, z;
      off += adv_dio_read_float64(doc, off, &x);
      off += adv_dio_read_float64(doc, off, &y);
      off += adv_dio_read_float64(doc, off, &z);
      nodes[i].x = x;
      nodes[i].y = y;
      nodes[i].z = z;
      if (num < 100 || i % (num/100) == 0)
	status_message("reading nodes... %d/%d\n", i, num);
    }
  status_message("reading nodes... done\n\n");

  nodes_did = adv_dio_get_documentid(doc);
  return true;
}

bool
Mesh::ReadElems(AdvDocument* doc)
{
  int32 num;
  int32 npe;
  const char* elemtype;
  int i;
  
  adv_dio_get_property_int32(doc, "num_items", &num);
  adv_dio_get_property_int32(doc, "num_nodes_per_element", &npe);
  elemtype = adv_dio_get_property(doc, "element_type");

  element_type = elemtype;
  if (strcmp(elemtype, "3DLinearHexahedron") == 0 && npe == 8)
    {
      num_nodes_per_elem = npe;
      num_faces_per_elem = 6;
      num_nodes_per_face = 4;
    }
  else if (strcmp(elemtype, "3DQuadraticTetrahedron") == 0 && npe == 10)
    {
      num_nodes_per_elem = npe;
      num_faces_per_elem = 4;
      num_nodes_per_face = 6;
    }
  else if (strcmp(elemtype, "3DLinearTriangle") == 0 && npe == 3)
   {
      num_nodes_per_elem = npe;
      num_faces_per_elem = 1;
      num_nodes_per_face = 3;
    }
  else
    {
      printf("unknown element type [%s]\n", elemtype);
      return false;
    }
      
  delete elems;
  num_elems = num;
  elems = new index_t[npe*num];

  adv_off_t off = 0;
  for (i = 0; i < num; i++)
    {
      off += adv_dio_read_int32v(doc, off, npe, elems+i*npe);
      if (num < 100 || i % (num/100) == 0)
	status_message("reading elems... %d/%d\n", i, num);
    }
  status_message("reading elems... done\n\n");

  ExtractSurfaces();

  elems_did = adv_dio_get_documentid(doc);
  return true;
}

static index_t* sort_face_array;
static int sort_face_size;
static int sort_cell_size;

static
int
comp_face(const index_t* a, const index_t* b)
{
  for (int i = 0; i < sort_face_size; i++)
    if (a[i] != b[i])
      return a[i]-b[i];
  return 0;
}

static
int
comp_face_index(const index_t* a, const index_t* b)
{
  return comp_face(sort_face_array + sort_cell_size*(*a), sort_face_array + sort_cell_size*(*b));
}

static
int
comp_index(const index_t* a, const index_t* b)
{
  return *a - *b;
}

void
Mesh::ExtractSurfaces()
{
  message("extracting surfaces...\n");

  static index_t faceindex_3dlh[] =
  {
    3, 2, 1, 0,
    7, 3, 0, 4,
    6, 2, 3, 7,
    5, 1, 2, 6,
    5, 4, 0, 1,
    4, 5, 6, 7,
  };
  static index_t faceindex_3dqt[] =
  {
    0, 6, 3, 8, 2, 5,
    1, 4, 0, 5, 2, 7,
    2, 8, 3, 9, 1, 7,
    0, 4, 1, 9, 3, 6,
  };
  static index_t faceindex_3dl_tri[] =
  {
    0, 1, 2,
  };
  index_t* faceindex;
  int checknode = 0;
  int nodestep = 0;
  bool do_extract;
  if (element_type == "3DLinearHexahedron")
    {
      do_extract = true;
      faceindex = faceindex_3dlh;
      checknode = 2;
      nodestep = 1;
    }
  else if (element_type == "3DQuadraticTetrahedron")
    {
      do_extract = true;
      faceindex = faceindex_3dqt;
      checknode = 3;
      nodestep = 2;
    }
#if 0
  else if (element_type == "3DLinearTriangle")
    {
      do_extract = false;
      faceindex = faceindex_3dl_tri;
    }
#endif
  else
    return;
  
  delete faces;
  faces = NULL;

  int csize = num_nodes_per_face;
  int num_faces = num_elems * num_faces_per_elem;

  index_t* work  = new index_t[num_faces * csize];
  int8*    mark  = new int8[num_faces];

  for (int i = 0; i < num_elems; i++)
    {
      index_t* elem = elems + i*num_nodes_per_elem;
      for (int j = 0; j < num_faces_per_elem; j++)
	{
	  mark[i*num_faces_per_elem + j] = 1;

	  int min_nodeidx = 0;
	  index_t min_node = elem[faceindex[j*num_nodes_per_face+min_nodeidx]];
	  for (int k = 0+nodestep; k < num_nodes_per_face; k+=nodestep)
	    if (elem[faceindex[j*num_nodes_per_face+k]] < min_node)
	      {
		min_nodeidx = k;
		min_node = elem[faceindex[j*num_nodes_per_face+k]];
	      }

	  for (int k = 0; k < num_nodes_per_face; k++)
	    {
	      int a = (k+min_nodeidx)%num_nodes_per_face;
	      work[(i*num_faces_per_elem + j)*num_nodes_per_face + k]
		= elem[faceindex[j*num_nodes_per_face+a]];
	    }
	}
      if (num_elems < 100 || i % (num_elems/100) == 0)
      	status_message("setup work area ... %d / %d\n", i, num_elems);
    }
  status_message("setup work area ... done!\n\n");

  message("sorting...\n");
  message("sorting stage 1...\n");

  index_t* m = new index_t[num_faces];
  index_t* n = new index_t[num_faces];
  for (int i = 0; i < num_faces; i++)
    m[i] = 0;
  for (int i = 0; i < num_faces; i++)
    {
      m[work[csize*i]]++;
      if (num_faces < 100 || i % (num_faces/100) == 0)
	status_message("sorting stage 1: count index ... %d / %d\n", i, num_faces);
    }
  status_message("sorting stage 1: count index ... done.\n\n");

  index_t mm = m[0];
  m[0] = 0;
  for (int i = 1; i < num_faces; i++)
    {
      index_t a;
      a = m[i];
      m[i] = m[i-1] + mm;
      mm = a;
      if (num_faces < 100 || i % (num_faces/100) == 0)
	status_message("sorting stage 1: make M buffer ... %d / %d\n", i, num_faces);
    }
  status_message("sorting stage 1: make M buffer ... done.\n\n");
	  
  for (int i = 0; i < num_faces; i++)
    {
      index_t a = work[csize*i];
      n[m[a]] = i;
      m[a]++;
      if (num_faces < 100 || i % (num_faces/100) == 0)
	status_message("sorting stage 1: make N buffer ... %d / %d\n", i, num_faces);
    }
  message("sorting stage 1: make N buffer ... done.\n");
  message("sorting stage 1: done.\n");

  message("sorting stage 2...\n");

  num_surfaces = 0;
  num_innfaces = 0;
  int headindex = 0;
  for (int i = 0; i < num_faces; )
    {
      int len = m[headindex] - (headindex > 0 ? m[headindex-1] : 0);
      if (len > 1)
	{
	  for (int j = 0; j < len; j++)
	    {
	      if (mark[n[i+j]] >= 0)
		{
		  int k;
		  for (k = j+1; k < len; k++)
		    if ((work[n[i+j]*num_nodes_per_face + 1]
			 == work[n[i+k]*num_nodes_per_face + num_nodes_per_face - 1])
			&& (work[n[i+j]*num_nodes_per_face + checknode]
			    == work[n[i+k]*num_nodes_per_face + num_nodes_per_face - checknode]))
		      {
			mark[n[i+j]] = -1;
			mark[n[i+k]] = -2;
			num_innfaces++;
			break;
		      }

		}

#if 0
	      if (work[n[i+j]*num_nodes_per_face + 0] == 170)
		{
		  printf("[%d] ", mark[n[i+j]]);
		  for (int k = 0; k < num_nodes_per_face; k++)
		    printf("%d ", work[n[i+j]*num_nodes_per_face + k]);
		  printf("\n");
		}
#endif
	    }
	}
      i = m[headindex];
      headindex++;
      if (num_faces < 100 || i % (num_faces/100) == 0)
	status_message("sorting stage 2: marking ... %d / %d\n", i, num_faces);
    }
  num_surfaces = num_faces - num_innfaces*2;
  status_message("sorting stage 2: marking ... done\n\n");

  message("face elimination %d -> %d, %d\n", num_faces, num_surfaces, num_innfaces);

  delete m;
  delete n;

  m = NULL;
  n = NULL;

  faces = new index_t[(num_surfaces+num_innfaces)*num_nodes_per_face];
  index_t* s = faces;
  int ss = 0;
  for (int i = 0; i < num_faces; i++)
    {
      index_t* w = work + i*csize;
	  
      if (mark[i] > 0)
	{
	  for (int j = 0; j < num_nodes_per_face; j++)
	    s[j] = w[j];
	  s += num_nodes_per_face;
	  ss++;
	}
      if (num_faces < 100 || i % (num_faces/100) == 0)
	status_message("sorting stage 2: scan surface ... %d / %d\n", i, num_faces);
    }
  status_message("sorting stage 2: scan surface ... done\n\n");
  num_surfaces = ss;
  ss = 0;
  for (int i = 0; i < num_faces; i++)
    {
      index_t* w = work + i*csize;
	  
      if (mark[i] == -1)
	{
	  for (int j = 0; j < num_nodes_per_face; j++)
	    s[j] = w[j];
	  s += num_nodes_per_face;
	  ss++;
	}
      if (num_faces < 100 || i % (num_faces/100) == 0)
	status_message("sorting stage 2: scan innface ... %d / %d\n", i, num_faces);
    }
  status_message("sorting stage 2: scan innface ... done\n\n");
  num_innfaces = ss;
  
  delete work;
  delete mark;

  message("sorting stage 2: done.\n");
}

bool
Mesh::ReadBCs(AdvDocument* doc)
{
#if  0
  int32 num;
  
  adv_dio_get_property_int32(doc, "items", &num);
  if (num == 0)
    return false;

  BCondGroup* bcg = BCondGroup::Create();
  bcg->type = adv_dio_get_property(doc, "content-type");
  adv_off_t off = 0;
  for (int i = 0; i < num; i++)
    {
      BCond bc;
      off += adv_dio_read_int32(doc, off, &bc.idx);
      off += adv_dio_read_int32(doc, off, &bc.axis);
      off += adv_dio_read_float64(doc, off, &bc.value);
      bcg->bconds.push_back(bc);
    }
  bcondgroups.push_back(bcg);
  BCondGroup::Add(bcg);
  return true;
#endif
}
