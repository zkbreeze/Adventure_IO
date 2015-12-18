/***********************************************************
Copyright (C) 2000,2001,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

//
// $Id: m8bizer.cc,v 1.2.2.1 2006/02/02 10:05:45 tohyama Exp $
//

#include <Adv/AdvFileIO.h>
#include <Adv/AdvUtil.h>
#include <Adv/AdvStat.h>
#include <unistd.h>
#include <stdio.h>
#include <vector>
#include <map>
#include <algo.h>
#include "mesh.h"
#include <Adv/advsys-config.h>
#include <stream.h>

extern "C" {
#include <term.h>
#if HAVE_CURSES == 1
#include <ncurses.h>
#elif HAVE_CURSES == 2
#include <curses.h>
#else
#error unknown of no curses lib.
#endif
};

struct
{
  bool verbose;
  bool sninfo;
  int terminfo_avail;
  char* outname;
} flags;
float64 start_time;

//======================================================================

template <typename T>
inline
void
sort(T& a, T& b)
{ if (b < a) sort(a, b); }


void
message(const char* fmt, ...)
{
  va_list va;
  if (flags.verbose)
    {
      float64 now;
      adv_query_time(&now);
      printf("[%8.1f] ", now-start_time);
      va_start(va, fmt);
      vprintf(fmt, va);
      va_end(va);
    }
}

void
status_message(const char* fmt, ...)
{
  va_list va;
  if (flags.verbose)
    {
      float64 now;
      adv_query_time(&now);
      printf("[%8.1f] ", now-start_time);
      va_start(va, fmt);
      vprintf(fmt, va);
      va_end(va);
      if (flags.terminfo_avail)
	{
	  char* str;
	  char* eseq;
	  str = tigetstr("cuu1");
	  tputs(str, fileno(stdout), putchar);
	  str = tigetstr("hpa");
	  eseq = tparm(str, 0);
	  tputs(eseq, fileno(stdout), putchar);
	  str = tigetstr("el");
	  tputs(str, fileno(stdout), putchar);
	}
    }
}

//======================================================================

class NodeGen
{
protected:
  NodeGen()
    {}
  virtual ~NodeGen()
    {}

public:
  virtual index_t EdgeCenter(index_t, index_t)
    { return -1; }
  virtual index_t FaceCenter(index_t, index_t, index_t, index_t)
    { return -1; }
  virtual index_t FaceCenter(index_t)
    { return -1; }
  virtual index_t BodyCenter(index_t)
    { return -1; }

  virtual void DumpNodes(AdvDocument* doc)
    { }
 
  virtual index_t NumNodes()
    { return -1; }

  static NodeGen* Create(Mesh* mesh);
  static void Destroy(NodeGen* ng);
};

class NodeGen_3DLH : public NodeGen
{
protected:
  friend NodeGen;
  NodeGen_3DLH(Mesh* m);
  virtual ~NodeGen_3DLH();

public:
  index_t EdgeCenter(index_t, index_t);
  index_t FaceCenter(index_t, index_t, index_t, index_t);
  index_t FaceCenter(index_t);
  index_t BodyCenter(index_t);

  void DumpNodes(AdvDocument* doc);
 
  index_t NumNodes()
    { return num_bodynodes+num_facenodes+num_edgenodes+mesh->num_nodes; }

private:
  Mesh* mesh;
  index_t bodynode_head;
  index_t num_bodynodes;
  index_t facenode_head;
  index_t num_facenodes;
  index_t edgenode_head;
  index_t num_edgenodes;
  vector< list<index_t*> > facenodemap;

  index_t* envector;
  index_t** enhead;
  index_t* enhead_last;
  index_t num_enhead;
};

class NodeGen_3DQT : public NodeGen
{
protected:
  friend NodeGen;
  NodeGen_3DQT(Mesh* m);
  virtual ~NodeGen_3DQT();

public:
  index_t EdgeCenter(index_t, index_t);
  index_t FaceCenter(index_t, index_t, index_t, index_t);
  index_t FaceCenter(index_t);
  index_t BodyCenter(index_t);

  void DumpNodes(AdvDocument* doc);
 
  index_t NumNodes()
    { return num_bodynodes+num_facenodes+num_edgenodes+mesh->num_nodes; }

private:
  Mesh* mesh;
  index_t bodynode_head;
  index_t num_bodynodes;
  index_t facenode_head;
  index_t num_facenodes;
  index_t edgenode_head;
  index_t num_edgenodes;
  vector< list<index_t*> > facenodemap;

  index_t* envector;
  index_t** enhead;
  index_t* enhead_last;
  index_t num_enhead;
};

//------------------------------------------------------------

NodeGen_3DLH::NodeGen_3DLH(Mesh* m)
  : mesh(m)
{
  mesh->Ref();
  bodynode_head = mesh->num_nodes;
  num_bodynodes = mesh->num_elems;
  facenode_head = bodynode_head+num_bodynodes;
  num_facenodes = mesh->num_surfaces+mesh->num_innfaces;

  vector< list<index_t> > enmap;

  int lastnode = edgenode_head = facenode_head+num_facenodes;
  num_edgenodes = 0;
  index_t* face = mesh->faces;
  for (int i = 0; i < mesh->num_surfaces+mesh->num_innfaces; i++)
    {
      int n = face[0] < face[1] ? face[0] : face[1];
      n = face[2] < n ? face[2] : n;
      n = face[3] < n ? face[3] : n;
      if (!(n < facenodemap.size()))
	{
	  for (int j = facenodemap.size(); j <= n; j++)
	    {
	      list<index_t*> a;
	      facenodemap.push_back(a);
	    }
	}
      list<index_t*>& fmap = facenodemap[n];
      fmap.push_back(face);

      for (int j = 0; j < 4; j++)
	{
	  index_t n1 = face[j];
	  index_t n2 = face[(j+1)%4];
	  if (n2 < n1)
	    swap(n1, n2);

	  if (!(n1 < enmap.size()))
	    {
	      for (int j = enmap.size(); j <= n1; j++)
		{
		  list<index_t> a;
		  enmap.push_back(a);
		}
	    }
	  list<index_t>& nlist = enmap[n1];
	  if (find(nlist.begin(), nlist.end(), n2) == nlist.end())
	    {
	      nlist.push_back(n2);
	      num_edgenodes++;
	    }
	}
      face += 4;
      if (i % (mesh->num_surfaces+mesh->num_innfaces / 100) == 0)
	status_message("nodegen: making face nodes and edge nodes ... %d/%d\n", i, mesh->num_surfaces+mesh->num_innfaces);
    }
  envector = new index_t[num_edgenodes];
  num_enhead = enmap.size()+1;
  enhead = new (index_t*)[num_enhead];
  int idx = 0;
  int nidx = 0;
  for (vector< list<index_t> >::iterator i = enmap.begin(); i != enmap.end(); i++)
    {
      enhead[nidx++] = envector+idx;
      for (list<index_t>::iterator j = i->begin(); j != i->end(); j++)
	envector[idx++] = *j;
    }
  enhead[nidx++] = envector+idx;
  enhead_last = envector+idx;
  status_message("nodegen: making face nodes and edge nodes ... done\n\n");
}

NodeGen_3DLH::~NodeGen_3DLH()
{
  mesh->Unref();
  delete[] envector;
  delete[] enhead;
}

index_t
NodeGen_3DLH::EdgeCenter(index_t a, index_t b)
{
  if (b < a)
    swap(a, b);

  index_t* head = enhead[a];
  index_t* tail = enhead[a+1];
  if (a+1 >= num_enhead)
    tail = enhead_last;
  for (index_t* p = head; p != tail; p++)
    if (*p == b)
      return edgenode_head + p-envector;
  throw(form("cannot find %d:%d\n", a, b));
}

index_t
NodeGen_3DLH::FaceCenter(index_t a, index_t b, index_t c, index_t d)
{
  index_t n = a < b ? a : b;
  n = c < n ? c : n;
  n = d < n ? d : n;
  list<index_t*>::iterator itor = facenodemap[n].begin();
  for (list<index_t*>::iterator itor = facenodemap[n].begin();
       itor != facenodemap[n].end(); itor++)
    {
      index_t* face = *itor;
      if (face[0] == a || face[1] == a || face[2] == a || face[3] == a)
	{
	  if ((face[0] == b || face[1] == b || face[2] == b || face[3] == b)
	      && (face[0] == c || face[1] == c || face[2] == c || face[3] == c)
	      && (face[0] == d || face[1] == d || face[2] == d || face[3] == d))
	    return FaceCenter((face-mesh->faces) / 4);
	}
    }
}

index_t
NodeGen_3DLH::FaceCenter(index_t idx)
{
  return idx+facenode_head;
}

index_t
NodeGen_3DLH::BodyCenter(index_t idx)
{
  return idx+bodynode_head;
}

void
NodeGen_3DLH::DumpNodes(AdvDocument* doc)
{
  message("dumping nodes...\n");

  adv_dio_set_property(doc, "content-type", "nodes");
  adv_dio_set_property_int32(doc, "items", mesh->num_nodes+num_bodynodes+num_facenodes+num_edgenodes);

  adv_dio_set_property(doc, "content_type", "Node");
  adv_dio_set_property_int32(doc, "dimension", 3);
  adv_dio_set_property(doc, "format", "f8f8f8");
  adv_dio_set_property_int32(doc, "num_items", mesh->num_nodes+num_bodynodes+num_facenodes+num_edgenodes);
  adv_off_t off = 0;
  for (int i = 0; i < mesh->num_nodes; i++)
    {
      off += adv_dio_write_float64(doc, off, mesh->nodes[i].x);
      off += adv_dio_write_float64(doc, off, mesh->nodes[i].y);
      off += adv_dio_write_float64(doc, off, mesh->nodes[i].z);
      if (mesh->num_nodes < 100 || i % (mesh->num_nodes/100) == 0)
	status_message("dumping nodes: original... %d/%d\n", i, mesh->num_nodes);
    }
  status_message("dumping nodes: original... done\n\n");

  for (int i = 0; i < num_bodynodes; i++)
    {
      double x = 0.0;
      double y = 0.0;
      double z = 0.0;
      for (int j = 0; j < mesh->num_nodes_per_elem; j++)
	{
	  x += mesh->nodes[mesh->elems[i*8 + j]].x;
	  y += mesh->nodes[mesh->elems[i*8 + j]].y;
	  z += mesh->nodes[mesh->elems[i*8 + j]].z;
	}
      x /= mesh->num_nodes_per_elem;
      y /= mesh->num_nodes_per_elem;
      z /= mesh->num_nodes_per_elem;
      off += adv_dio_write_float64(doc, off, x);
      off += adv_dio_write_float64(doc, off, y);
      off += adv_dio_write_float64(doc, off, z);
      if (num_bodynodes < 100 || i % (num_bodynodes/100) == 0)
	status_message("dumping nodes: body nodes... %d/%d\n", i, num_bodynodes);
    }
  status_message("dumping nodes: body nodes... done\n\n");

  for (int i = 0; i < num_facenodes; i++)
    {
      double x = 0.0;
      double y = 0.0;
      double z = 0.0;
      for (int j = 0; j < 4; j++)
	{
	  x += mesh->nodes[mesh->faces[i*4 + j]].x;
	  y += mesh->nodes[mesh->faces[i*4 + j]].y;
	  z += mesh->nodes[mesh->faces[i*4 + j]].z;
	}
      x *= 0.25;
      y *= 0.25;
      z *= 0.25;
      off += adv_dio_write_float64(doc, off, x);
      off += adv_dio_write_float64(doc, off, y);
      off += adv_dio_write_float64(doc, off, z);
      if (num_facenodes < 100 || i % (num_facenodes/100) == 0)
	status_message("dumping nodes: face nodes... %d/%d\n", i, num_facenodes);
    }
  status_message("dumping nodes: face nodes... done\n\n");

  for (int i = 0; i < num_enhead-1; i++)
    {
      index_t* head = enhead[i];
      index_t* tail = enhead[i+1];
      for (index_t* p = head; p != tail; p++)
	{
	  int j = *p;
	  int n = edgenode_head + p - envector;
	  double x = (mesh->nodes[i].x + mesh->nodes[j].x) * 0.5;
	  double y = (mesh->nodes[i].y + mesh->nodes[j].y) * 0.5;
	  double z = (mesh->nodes[i].z + mesh->nodes[j].z) * 0.5;

	  off += adv_dio_write_float64(doc, sizeof(float64)*(n*3+0), x);
	  off += adv_dio_write_float64(doc, sizeof(float64)*(n*3+1), y);
	  off += adv_dio_write_float64(doc, sizeof(float64)*(n*3+2), z);
	}
      if (num_enhead < 100 || i % (num_enhead/100) == 0)
	status_message("dumping nodes: edge nodes... %d/%d\n", i, num_enhead);
    }
  status_message("dumping nodes: edge nodes... done\n\n");
}

//============================================================

NodeGen_3DQT::NodeGen_3DQT(Mesh* m)
  : mesh(m)
{
  mesh->Ref();

  bodynode_head = mesh->num_nodes;
  num_bodynodes = mesh->num_elems;
  facenode_head = bodynode_head+num_bodynodes;
  num_facenodes = 0;

  int lastnode = edgenode_head = facenode_head+num_facenodes;
  num_edgenodes = 0;
  index_t* face = mesh->faces;

  vector< list<index_t> > enmap;
      
  for (int i = 0; i < mesh->num_surfaces+mesh->num_innfaces; i++)
    {
      int n = face[0] < face[1] ? face[0] : face[1];
      n = face[2] < n ? face[2] : n;
      n = face[3] < n ? face[3] : n;
      n = face[4] < n ? face[4] : n;
      n = face[5] < n ? face[5] : n;
      if (!(n < facenodemap.size()))
	{
	  for (int j = facenodemap.size(); j <= n; j++)
	    {
	      list<index_t*> a;
	      facenodemap.push_back(a);
	    }
	}
      list<index_t*>& fmap = facenodemap[n];
      fmap.push_back(face);

      for (int j = 0; j < 6+3; j++)
	{
	  index_t n1, n2;
	  if (j < 6)
	    {
	      n1 = face[j];
	      n2 = face[(j+1)%6];
	    }
	  else
	    {
	      n1 = face[1+  (j-6)*2   ];
	      n2 = face[1+ ((j-5)*2)%6];
	    }
	      
	  if (n2 < n1)
	    swap(n1, n2);

	  if (!(n1 < enmap.size()))
	    {
	      for (int j = enmap.size(); j <= n1; j++)
		{
		  list<index_t> a;
		  enmap.push_back(a);
		}
	    }
	  list<index_t>& nlist = enmap[n1];
	  if (find(nlist.begin(), nlist.end(), n2) == nlist.end())
	    {
	      nlist.push_back(n2);
	      num_edgenodes++;
	    }
	}
      face += 6;
      if (i % (mesh->num_surfaces+mesh->num_innfaces / 100) == 0)
	status_message("nodegen: making face nodes and edge nodes ... %d/%d\n",
		       i, mesh->num_surfaces+mesh->num_innfaces);
    }
  envector = new index_t[num_edgenodes];
  num_enhead = enmap.size()+1;
  enhead = new (index_t*)[num_enhead];
  int idx = 0;
  int nidx = 0;
  for (vector< list<index_t> >::iterator i = enmap.begin(); i != enmap.end(); i++)
    {
      enhead[nidx++] = envector+idx;
      for (list<index_t>::iterator j = i->begin(); j != i->end(); j++)
	{
	  envector[idx++] = *j;
	}
    }
  enhead[nidx++] = envector+idx;
  enhead_last = envector+idx;
  status_message("nodegen: making face nodes and edge nodes ... done\n\n");
}

NodeGen_3DQT::~NodeGen_3DQT()
{
  mesh->Unref();
  delete[] envector;
  delete[] enhead;
}

index_t
NodeGen_3DQT::EdgeCenter(index_t a, index_t b)
{
  if (b < a)
    swap(a, b);

  index_t* head = enhead[a];
  index_t* tail = enhead[a+1];
  if (a+1 >= num_enhead)
    tail = enhead_last;
  for (index_t* p = head; p != tail; p++)
    if (*p == b)
      return edgenode_head + p-envector;
  throw (form("cannot find %d:%d\n", a, b));
}

index_t
NodeGen_3DQT::FaceCenter(index_t a, index_t b, index_t c, index_t d)
{
  return -1;
}

index_t
NodeGen_3DQT::FaceCenter(index_t idx)
{
  return -1;
}

index_t
NodeGen_3DQT::BodyCenter(index_t idx)
{
  return idx+bodynode_head;
}

void
NodeGen_3DQT::DumpNodes(AdvDocument* doc)
{
  message("dumping nodes...\n");

  adv_dio_set_property(doc, "content-type", "nodes");
  adv_dio_set_property_int32(doc, "items", mesh->num_nodes+num_bodynodes+num_facenodes+num_edgenodes);

  adv_dio_set_property(doc, "content_type", "Node");
  adv_dio_set_property_int32(doc, "dimension", 3);
  adv_dio_set_property(doc, "format", "f8f8f8");
  adv_dio_set_property_int32(doc, "num_items", mesh->num_nodes+num_bodynodes+num_facenodes+num_edgenodes);
  adv_off_t off = 0;
  for (int i = 0; i < mesh->num_nodes; i++)
    {
      off += adv_dio_write_float64(doc, off, mesh->nodes[i].x);
      off += adv_dio_write_float64(doc, off, mesh->nodes[i].y);
      off += adv_dio_write_float64(doc, off, mesh->nodes[i].z);
      if (mesh->num_nodes < 100 || i % (mesh->num_nodes/100) == 0)
	status_message("dumping nodes: original... %d/%d\n", i, mesh->num_nodes);
    }
  status_message("dumping nodes: original... done\n\n");

  for (int i = 0; i < num_bodynodes; i++)
    {
      double x = 0.0;
      double y = 0.0;
      double z = 0.0;
#if 0      
      for (int j = 0; j < mesh->num_nodes_per_elem; j++)
	{
	  x += mesh->nodes[mesh->elems[i*8 + j]].x;
	  y += mesh->nodes[mesh->elems[i*8 + j]].y;
	  z += mesh->nodes[mesh->elems[i*8 + j]].z;
	}
      x /= mesh->num_nodes_per_elem;
      y /= mesh->num_nodes_per_elem;
      z /= mesh->num_nodes_per_elem;
#else
      int n = mesh->num_nodes_per_elem;
      x = mesh->nodes[mesh->elems[i*n + 5]].x + mesh->nodes[mesh->elems[i*n + 9]].x;
      y = mesh->nodes[mesh->elems[i*n + 5]].y + mesh->nodes[mesh->elems[i*n + 9]].y;
      z = mesh->nodes[mesh->elems[i*n + 5]].z + mesh->nodes[mesh->elems[i*n + 9]].z;
      x *= 0.5;
      y *= 0.5;
      z *= 0.5;
#endif
      off += adv_dio_write_float64(doc, off, x);
      off += adv_dio_write_float64(doc, off, y);
      off += adv_dio_write_float64(doc, off, z);
      if (num_bodynodes < 100 || i % (num_bodynodes/100) == 0)
	status_message("dumping nodes: body nodes... %d/%d\n", i, num_bodynodes);
    }
  status_message("dumping nodes: body nodes... done\n\n");

  for (int i = 0; i < num_facenodes; i++)
    {
      double x = 0.0;
      double y = 0.0;
      double z = 0.0;
      for (int j = 0; j < 4; j++)
	{
	  x += mesh->nodes[mesh->faces[i*4 + j]].x;
	  y += mesh->nodes[mesh->faces[i*4 + j]].y;
	  z += mesh->nodes[mesh->faces[i*4 + j]].z;
	}
      x *= 0.25;
      y *= 0.25;
      z *= 0.25;
      off += adv_dio_write_float64(doc, off, x);
      off += adv_dio_write_float64(doc, off, y);
      off += adv_dio_write_float64(doc, off, z);
      if (num_facenodes < 100 || i % (num_facenodes/100) == 0)
	status_message("dumping nodes: face nodes... %d/%d\n", i, num_facenodes);
    }
  status_message("dumping nodes: face nodes... done\n\n");

  for (int i = 0; i < num_enhead-1; i++)
    {
      index_t* head = enhead[i];
      index_t* tail = enhead[i+1];
      for (index_t* p = head; p != tail; p++)
	{
	  int j = *p;
	  int n = edgenode_head + p - envector;
	  //printf("%d : %d %d \n", n, i, j);
	  double x = (mesh->nodes[i].x + mesh->nodes[j].x) * 0.5;
	  double y = (mesh->nodes[i].y + mesh->nodes[j].y) * 0.5;
	  double z = (mesh->nodes[i].z + mesh->nodes[j].z) * 0.5;

	  off += adv_dio_write_float64(doc, sizeof(float64)*(n*3+0), x);
	  off += adv_dio_write_float64(doc, sizeof(float64)*(n*3+1), y);
	  off += adv_dio_write_float64(doc, sizeof(float64)*(n*3+2), z);
	}
      if (num_enhead < 100 || i % (num_enhead/100) == 0)
	status_message("dumping nodes: edge nodes... %d/%d\n", i, num_enhead);
    }
  status_message("dumping nodes: edge nodes... done\n\n");
}

//============================================================

NodeGen*
NodeGen::Create(Mesh* mesh)
{
  if (mesh->element_type == "3DLinearHexahedron")
    return new NodeGen_3DLH(mesh);
  else if (mesh->element_type == "3DQuadraticTetrahedron")
    return new NodeGen_3DQT(mesh);
  else
    return NULL;
}

void
NodeGen::Destroy(NodeGen* ng)
{ delete ng; }

//============================================================

struct BCond
{
  int8 coord;					  // ORs of 0x1:x, 0x2:y, 0x4:z
  float64 value[3];
  
  BCond()
    : coord(0) {}
};

class CondGen
{
public:
  CondGen(int os, int ds);
  virtual ~CondGen();

  virtual bool ReadDoc(AdvDocument* doc);
  virtual void DumpCond(AdvDocument* doc, Mesh* mesh, NodeGen* ngen);

protected:
  int osize;
  int dsize;
  bit_vector idxmap;
  string fgc_type;
  string label;
  vector<BCond*> values;
};

class CondGen_3DLH : public CondGen
{
public:
  CondGen_3DLH(int os, int ds);
  virtual ~CondGen_3DLH();

  bool ReadDoc(AdvDocument* doc);
  void DumpCond(AdvDocument* doc, Mesh* mesh, NodeGen* ngen);
};

class CondGen_3DQT : public CondGen
{
public:
  CondGen_3DQT(int os, int ds);
  virtual ~CondGen_3DQT();

  bool ReadDoc(AdvDocument* doc);
  void DumpCond(AdvDocument* doc, Mesh* mesh, NodeGen* ngen);
};

//------------------------------------------------------------

CondGen::CondGen(int original_size, int divided_size)
  : osize(original_size)
  , dsize(divided_size)
{
  idxmap.resize(dsize, false);
  values.resize(osize, NULL);
}

CondGen::~CondGen()
{
}

bool
CondGen::ReadDoc(AdvDocument* doc)
{
  const char* type = adv_dio_get_property(doc, "fega_type");
  if (strcmp(type, "NodeVariable") != 0 && strcmp(type, "NodeConstant") != 0)
    return false;
  fgc_type = type;

  const char* format = adv_dio_get_property(doc, "format");
  if (!(    (fgc_type == "NodeVariable" && strcmp(format, "i4f8") == 0)
	 || (fgc_type == "NodeConstant" && strcmp(format, "i1f8f8f8") == 0) ))
    return false;

  label = adv_dio_get_property(doc, "label");
  message("FEGA[%s]: %s\n", fgc_type.c_str(), label.c_str());

  int num;
  adv_dio_get_property_int32(doc, "num_items", &num);
  message("  initial items %d\n", num);

  for (vector<BCond*>::iterator i = values.begin(); i != values.end(); i++)
    if (*i)
      delete (*i);
  for (int i = 0; i < values.size(); i++)
    values[i] = NULL;
  for (int i = 0; i < dsize; i++)
    idxmap[i] = false;

  if (fgc_type == "NodeVariable")
    for (int i = 0; i < num; i++)
      {
	int32 idx, coord;
	float64 value;
	adv_dio_read_int32(doc, i*(4+4+8), &idx);
	adv_dio_read_int32(doc, i*(4+4+8)+4, &coord);
	adv_dio_read_float64(doc, i*(4+4+8)+4+4, &value);
	idxmap[idx] = true;
	if (values[idx] == NULL)
	  {
	    BCond* bc = new BCond;
	    values[idx] = bc;
	  }
	values[idx]->coord |= (0x1 << coord);
	values[idx]->value[coord] = value;
      }
  else if (fgc_type == "NodeConstant")
    {
      adv_off_t off = 0;
      int8 coord;
      float64 value[3];
      off += adv_dio_read_int8(doc, off, &coord);
      off += adv_dio_read_float64(doc, off, value+0);
      off += adv_dio_read_float64(doc, off, value+1);
      off += adv_dio_read_float64(doc, off, value+2);

      values[0] = new BCond;
      values[0]->coord = coord;
      values[0]->value[0] = value[0];
      values[0]->value[1] = value[1];
      values[0]->value[2] = value[2];

      for (int i = 0; i < num; i++)
      {
	int32 idx;
	off += adv_dio_read_int32(doc, off, &idx);
	idxmap[idx] = true;
      }
    }
}

void
CondGen::DumpCond(AdvDocument* doc, Mesh* mesh, NodeGen* ngen)
{
  adv_off_t off = 0;
  index_t num = 0;
  if (fgc_type == "NodeVariable")
    {
      for (int i = 0; i < values.size(); i++)
	{
	  if (values[i])
	    {
	      int8 coord = values[i]->coord;
	      for (int j = 0; j < 3; j++)
		{
		  if (coord & (0x1 << j))
		    {
		      double v = values[i]->value[j];
		      off += adv_dio_write_int32(doc, off, i);
		      off += adv_dio_write_int32(doc, off, j);
		      off += adv_dio_write_float64(doc, off, v);
		      num++;
		    }
		}
	    }
	}
      if (mesh->element_type == "3DLinearHexahedron")
	{
	  for (int i = 0; i < mesh->num_surfaces; i++)
	    {
	      const index_t* face = mesh->faces + 4*i;
	      int count = 0;
	      for (int j = 0; j < 4; j++)
		if (idxmap[face[j]])
		  count++;
	      if (count >= 4)
		{
		  int fidx = ngen->FaceCenter(face[0], face[1], face[2], face[3]);
		  if (idxmap[fidx] == false)
		    {
		      idxmap[fidx] = true;
		      int8 coord = 0;
		      for (int j = 0; j < 4; j++)
			coord |= values[face[j]]->coord;
		      for (int j = 0; j < 3; j++)
			{
			  if (coord & (0x1 << j))
			    {
			      double v = 0.0;
			      for (int k = 0; k < 4; k++)
				v += values[face[k]]->value[j];
			      v *= 0.25;
			      off += adv_dio_write_int32(doc, off, fidx);
			      off += adv_dio_write_int32(doc, off, j);
			      off += adv_dio_write_float64(doc, off, v);
			      num++;
			    }
			}
		    }
		  for (int j = 0; j < 4; j++)
		    {
		      int eidx = ngen->EdgeCenter(face[j], face[(j+1)%4]);
		      if (idxmap[eidx] == false)
			{
			  idxmap[eidx] = true;
			  int8 coord = 0;
			  coord |= values[face[j]]->coord;
			  coord |= values[face[(j+1)%4]]->coord;
			  for (int k = 0; k < 3; k++)
			    {
			      if (coord & (0x1 << k))
				{
				  double v = values[face[j]]->value[k] + values[face[(j+1)%4]]->value[k];
				  v *= 0.5;
				  off += adv_dio_write_int32(doc, off, eidx);
				  off += adv_dio_write_int32(doc, off, k);
				  off += adv_dio_write_float64(doc, off, v);
				  num++;
				}
			    }
			}
		    }
		}
	    }
	}
      else if (mesh->element_type == "3DQuadraticTetrahedron")
	{
	  for (int i = 0; i < mesh->num_surfaces; i++)
	    {
	      const index_t* face = mesh->faces + 6*i;
	      int count = 0;
	      for (int j = 0; j < 6; j++)
		if (idxmap[face[j]])
		  count++;
	      if (count >= 6)
		{
		  for (int j = 0; j < 9; j++)
		    {
		      index_t n1, n2;
		      if (j < 6)
			{ n1 = face[j]; n2 = face[(j+1) %6]; }
		      else
			{ n1 = face[1+ (j-6)*2]; n2 = face[(3+ (j-6)*2) % 6]; }

		      int eidx = ngen->EdgeCenter(n1, n2);
		      if (idxmap[eidx] == false)
			{
			  idxmap[eidx] = true;
			  int8 coord = 0;
			  coord |= values[n1]->coord;
			  coord |= values[n2]->coord;
			  for (int k = 0; k < 3; k++)
			    {
			      if (coord & (0x1 << k))
				{
				  double v = values[n1]->value[k] + values[n2]->value[k];
				  v *= 0.5;
				  off += adv_dio_write_int32(doc, off, eidx);
				  off += adv_dio_write_int32(doc, off, k);
				  off += adv_dio_write_float64(doc, off, v);
				  num++;
				}
			    }
			}
		    }
		}
	    }
	}
      adv_dio_set_property(doc, "content_type", "FEGenericAttribute");
      adv_dio_set_property(doc, "fega_type", "NodeVariable");
      adv_dio_set_property(doc, "format", "i4f8");
      adv_dio_set_property(doc, "dd_option", "");
      adv_dio_set_property(doc, "index_byte", "4");
      adv_dio_set_property(doc, "label", label.c_str());
      adv_dio_set_property_int32(doc, "num_items", num);
    }
  if (fgc_type == "NodeConstant")
    {
      off += adv_dio_write_int8(doc, off, values[0]->coord);
      off += adv_dio_write_float64(doc, off, values[0]->value[0]);
      off += adv_dio_write_float64(doc, off, values[0]->value[1]);
      off += adv_dio_write_float64(doc, off, values[0]->value[2]);

      for (int i = 0; i < values.size(); i++)
	{
	  if (idxmap[i])
	    {
	      off += adv_dio_write_int32(doc, off, i);
	      num++;
	    }
	}
      if (mesh->element_type == "3DLinearHexahedron")
	{
	  for (int i = 0; i < mesh->num_surfaces; i++)
	    {
	      const index_t* face = mesh->faces + 4*i;
	      int count = 0;
	      for (int j = 0; j < 4; j++)
		if (idxmap[face[j]])
		  count++;
	      if (count >= 4)
		{
		  int fidx = ngen->FaceCenter(face[0], face[1], face[2], face[3]);
		  if (idxmap[fidx] == false)
		    {
		      idxmap[fidx] = true;
		      off += adv_dio_write_int32(doc, off, fidx);
		      num++;
		    }
		  for (int j = 0; j < 4; j++)
		    {
		      int eidx = ngen->EdgeCenter(face[j], face[(j+1)%4]);
		      if (idxmap[eidx] == false)
			{
			  idxmap[eidx] = true;
			  off += adv_dio_write_int32(doc, off, eidx);
			  num++;
			}
		    }
		}
	    }
	}
      else if (mesh->element_type == "3DQuadraticTetrahedron")
	{
	  for (int i = 0; i < mesh->num_surfaces; i++)
	    {
	      const index_t* face = mesh->faces + 6*i;
	      int count = 0;
	      for (int j = 0; j < 6; j++)
		if (idxmap[face[j]])
		  count++;
	      if (count >= 6)
		{
		  for (int j = 0; j < 9; j++)
		    {
		      index_t n1, n2;
		      if (j < 6)
			{ n1 = face[j]; n2 = face[(j+1) %6]; }
		      else
			{ n1 = face[1+ (j-6)*2]; n2 = face[(3+ (j-6)*2) % 6]; }

		      int eidx = ngen->EdgeCenter(n1, n2);
		      if (idxmap[eidx] == false)
			{
			  idxmap[eidx] = true;
			  off += adv_dio_write_int32(doc, off, eidx);
			  num++;
			}
		    }
		}
	    }
	}
      adv_dio_set_property(doc, "content_type", "FEGenericAttribute");
      adv_dio_set_property(doc, "fega_type", "NodeConstant");
      adv_dio_set_property(doc, "format", "i1f8f8f8");
      adv_dio_set_property(doc, "dd_option", "");
      adv_dio_set_property(doc, "index_byte", "4");
      adv_dio_set_property(doc, "label", label.c_str());
      adv_dio_set_property_int32(doc, "num_items", num);
    }
  message("  divided items %d\n", num);
}

//============================================================

void dump_elem(AdvDocument* doc, Mesh* mesh, NodeGen* ngen)
{
  if (mesh->element_type == "3DLinearHexahedron")
    {
      adv_off_t off = 0;
      adv_dio_set_property(doc, "content-type", "elements");
      adv_dio_set_property_int32(doc, "items", mesh->num_elems*8);
      adv_dio_set_property_int32(doc, "nodes_per_element", 8);

      adv_dio_set_property(doc, "content_type", "Element");
      adv_dio_set_property_int32(doc, "num_items", mesh->num_elems*8);
      adv_dio_set_property_int32(doc, "num_nodes_per_element", 8);
      adv_dio_set_property(doc, "format", "i4i4i4i4i4i4i4i4");
      adv_dio_set_property(doc, "element_type", "3DLinearHexahedron");
      adv_dio_set_property(doc, "dimension", "3");
      adv_dio_set_property(doc, "index_byte", "4");
  
      for (int idx = 0; idx < mesh->num_elems; idx++)
	{
	  index_t* elem = mesh->elems + idx*8;
	  int n[27];
	  n[0] = elem[0];
	  n[1] = ngen->EdgeCenter(elem[0], elem[3]);
	  n[2] = elem[3];
	  n[3] = ngen->EdgeCenter(elem[0], elem[1]);
	  n[4] = ngen->FaceCenter(elem[0], elem[1], elem[2], elem[3]);
	  n[5] = ngen->EdgeCenter(elem[2], elem[3]);
	  n[6] = elem[1];
	  n[7] = ngen->EdgeCenter(elem[1], elem[2]);
	  n[8] = elem[2];

	  n[9]  = ngen->EdgeCenter(elem[0], elem[4]);
	  n[10] = ngen->FaceCenter(elem[0], elem[3], elem[7], elem[4]);
	  n[11] = ngen->EdgeCenter(elem[3], elem[7]);
	  n[12] = ngen->FaceCenter(elem[0], elem[4], elem[5], elem[1]);
	  n[13] = ngen->BodyCenter(idx);
	  n[14] = ngen->FaceCenter(elem[6], elem[2], elem[3], elem[7]);
	  n[15] = ngen->EdgeCenter(elem[1], elem[5]);
	  n[16] = ngen->FaceCenter(elem[1], elem[2], elem[6], elem[5]);
	  n[17] = ngen->EdgeCenter(elem[2], elem[6]);

	  n[18] = elem[4];
	  n[19] = ngen->EdgeCenter(elem[4], elem[7]);
	  n[20] = elem[7];
	  n[21] = ngen->EdgeCenter(elem[5], elem[4]);
	  n[22] = ngen->FaceCenter(elem[4], elem[5], elem[6], elem[7]);
	  n[23] = ngen->EdgeCenter(elem[6], elem[7]);
	  n[24] = elem[5];
	  n[25] = ngen->EdgeCenter(elem[5], elem[6]);
	  n[26] = elem[6];

	  off += adv_dio_write_int32(doc, off, n[ 0]);
	  off += adv_dio_write_int32(doc, off, n[ 3]);
	  off += adv_dio_write_int32(doc, off, n[ 4]);
	  off += adv_dio_write_int32(doc, off, n[ 1]);
	  off += adv_dio_write_int32(doc, off, n[ 9]);
	  off += adv_dio_write_int32(doc, off, n[12]);
	  off += adv_dio_write_int32(doc, off, n[13]);
	  off += adv_dio_write_int32(doc, off, n[10]);
	  off += adv_dio_write_int32(doc, off, n[ 3]);
	  off += adv_dio_write_int32(doc, off, n[ 6]);
	  off += adv_dio_write_int32(doc, off, n[ 7]);
	  off += adv_dio_write_int32(doc, off, n[ 4]);
	  off += adv_dio_write_int32(doc, off, n[12]);
	  off += adv_dio_write_int32(doc, off, n[15]);
	  off += adv_dio_write_int32(doc, off, n[16]);
	  off += adv_dio_write_int32(doc, off, n[13]);
	  off += adv_dio_write_int32(doc, off, n[ 4]);
	  off += adv_dio_write_int32(doc, off, n[ 7]);
	  off += adv_dio_write_int32(doc, off, n[ 8]);
	  off += adv_dio_write_int32(doc, off, n[ 5]);
	  off += adv_dio_write_int32(doc, off, n[13]);
	  off += adv_dio_write_int32(doc, off, n[16]);
	  off += adv_dio_write_int32(doc, off, n[17]);
	  off += adv_dio_write_int32(doc, off, n[14]);
	  off += adv_dio_write_int32(doc, off, n[ 1]);
	  off += adv_dio_write_int32(doc, off, n[ 4]);
	  off += adv_dio_write_int32(doc, off, n[ 5]);
	  off += adv_dio_write_int32(doc, off, n[ 2]);
	  off += adv_dio_write_int32(doc, off, n[10]);
	  off += adv_dio_write_int32(doc, off, n[13]);
	  off += adv_dio_write_int32(doc, off, n[14]);
	  off += adv_dio_write_int32(doc, off, n[11]);
	  off += adv_dio_write_int32(doc, off, n[ 9]);
	  off += adv_dio_write_int32(doc, off, n[12]);
	  off += adv_dio_write_int32(doc, off, n[13]);
	  off += adv_dio_write_int32(doc, off, n[10]);
	  off += adv_dio_write_int32(doc, off, n[18]);
	  off += adv_dio_write_int32(doc, off, n[21]);
	  off += adv_dio_write_int32(doc, off, n[22]);
	  off += adv_dio_write_int32(doc, off, n[19]);
	  off += adv_dio_write_int32(doc, off, n[12]);
	  off += adv_dio_write_int32(doc, off, n[15]);
	  off += adv_dio_write_int32(doc, off, n[16]);
	  off += adv_dio_write_int32(doc, off, n[13]);
	  off += adv_dio_write_int32(doc, off, n[21]);
	  off += adv_dio_write_int32(doc, off, n[24]);
	  off += adv_dio_write_int32(doc, off, n[25]);
	  off += adv_dio_write_int32(doc, off, n[22]);
	  off += adv_dio_write_int32(doc, off, n[13]);
	  off += adv_dio_write_int32(doc, off, n[16]);
	  off += adv_dio_write_int32(doc, off, n[17]);
	  off += adv_dio_write_int32(doc, off, n[14]);
	  off += adv_dio_write_int32(doc, off, n[22]);
	  off += adv_dio_write_int32(doc, off, n[25]);
	  off += adv_dio_write_int32(doc, off, n[26]);
	  off += adv_dio_write_int32(doc, off, n[23]);
	  off += adv_dio_write_int32(doc, off, n[10]);
	  off += adv_dio_write_int32(doc, off, n[13]);
	  off += adv_dio_write_int32(doc, off, n[14]);
	  off += adv_dio_write_int32(doc, off, n[11]);
	  off += adv_dio_write_int32(doc, off, n[19]);
	  off += adv_dio_write_int32(doc, off, n[22]);
	  off += adv_dio_write_int32(doc, off, n[23]);
	  off += adv_dio_write_int32(doc, off, n[20]);
	  if (mesh->num_elems < 100 || idx % (mesh->num_elems/100) == 0)
	    status_message("dumping elems... %d/%d\n", idx, mesh->num_elems);
	}
      status_message("dumping elems... done\n\n");
    }
  else if (mesh->element_type == "3DQuadraticTetrahedron")
    {
      adv_off_t off = 0;
      adv_dio_set_property(doc, "content-type", "elements");
      adv_dio_set_property_int32(doc, "items", mesh->num_elems*8);
      adv_dio_set_property_int32(doc, "nodes_per_element", 10);

      adv_dio_set_property(doc, "content_type", "Element");
      adv_dio_set_property_int32(doc, "num_items", mesh->num_elems*8);
      adv_dio_set_property_int32(doc, "num_nodes_per_element", 10);
      adv_dio_set_property(doc, "format", "i4i4i4i4i4i4i4i4i4i4");
      adv_dio_set_property(doc, "element_type", "3DQuadraticTetrahedron");
      adv_dio_set_property(doc, "dimension", "3");
      adv_dio_set_property(doc, "index_byte", "4");
  
      for (int idx = 0; idx < mesh->num_elems; idx++)
	{
	  index_t* elem = mesh->elems + idx*10;
	  int n[35];
	  n[0] = elem[0];
	  n[1] = elem[1];
	  n[2] = elem[2];
	  n[3] = elem[3];
	  n[4] = elem[4];
	  n[5] = elem[5];
	  n[6] = elem[6];
	  n[7] = elem[7];
	  n[8] = elem[8];
	  n[9] = elem[9];

	  n[10] = ngen->EdgeCenter(elem[0], elem[4]);
	  n[11] = ngen->EdgeCenter(elem[0], elem[5]);
	  n[12] = ngen->EdgeCenter(elem[0], elem[6]);
	  n[13] = ngen->EdgeCenter(elem[4], elem[5]);
	  n[14] = ngen->EdgeCenter(elem[5], elem[6]);
	  n[15] = ngen->EdgeCenter(elem[6], elem[4]);

	  n[16] = ngen->EdgeCenter(elem[1], elem[7]);
	  n[17] = ngen->EdgeCenter(elem[1], elem[4]);
	  n[18] = ngen->EdgeCenter(elem[1], elem[9]);
	  n[19] = ngen->EdgeCenter(elem[7], elem[4]);
	  n[20] = ngen->EdgeCenter(elem[4], elem[9]);
	  n[21] = ngen->EdgeCenter(elem[9], elem[7]);

	  n[22] = ngen->EdgeCenter(elem[2], elem[7]);
	  n[23] = ngen->EdgeCenter(elem[2], elem[5]);
	  n[24] = ngen->EdgeCenter(elem[2], elem[8]);
	  n[25] = ngen->EdgeCenter(elem[7], elem[5]);
	  n[26] = ngen->EdgeCenter(elem[5], elem[8]);
	  n[27] = ngen->EdgeCenter(elem[8], elem[7]);

	  n[28] = ngen->EdgeCenter(elem[3], elem[8]);
	  n[29] = ngen->EdgeCenter(elem[3], elem[6]);
	  n[30] = ngen->EdgeCenter(elem[3], elem[9]);
	  n[31] = ngen->EdgeCenter(elem[8], elem[6]);
	  n[32] = ngen->EdgeCenter(elem[6], elem[9]);
	  n[33] = ngen->EdgeCenter(elem[9], elem[8]);

	  n[34] = ngen->BodyCenter(idx);

	  off += adv_dio_write_int32(doc, off, n[ 0]);
	  off += adv_dio_write_int32(doc, off, n[ 4]);
	  off += adv_dio_write_int32(doc, off, n[ 5]);
	  off += adv_dio_write_int32(doc, off, n[ 6]);
	  off += adv_dio_write_int32(doc, off, n[10]);
	  off += adv_dio_write_int32(doc, off, n[11]);
	  off += adv_dio_write_int32(doc, off, n[12]);
	  off += adv_dio_write_int32(doc, off, n[13]);
	  off += adv_dio_write_int32(doc, off, n[14]);
	  off += adv_dio_write_int32(doc, off, n[15]);

	  off += adv_dio_write_int32(doc, off, n[ 1]);
	  off += adv_dio_write_int32(doc, off, n[ 7]);
	  off += adv_dio_write_int32(doc, off, n[ 4]);
	  off += adv_dio_write_int32(doc, off, n[ 9]);
	  off += adv_dio_write_int32(doc, off, n[16]);
	  off += adv_dio_write_int32(doc, off, n[17]);
	  off += adv_dio_write_int32(doc, off, n[18]);
	  off += adv_dio_write_int32(doc, off, n[19]);
	  off += adv_dio_write_int32(doc, off, n[20]);
	  off += adv_dio_write_int32(doc, off, n[21]);

	  off += adv_dio_write_int32(doc, off, n[ 2]);
	  off += adv_dio_write_int32(doc, off, n[ 7]);
	  off += adv_dio_write_int32(doc, off, n[ 8]);
	  off += adv_dio_write_int32(doc, off, n[ 5]);
	  off += adv_dio_write_int32(doc, off, n[22]);
	  off += adv_dio_write_int32(doc, off, n[24]);
	  off += adv_dio_write_int32(doc, off, n[23]);
	  off += adv_dio_write_int32(doc, off, n[27]);
	  off += adv_dio_write_int32(doc, off, n[26]);
	  off += adv_dio_write_int32(doc, off, n[25]);

	  off += adv_dio_write_int32(doc, off, n[ 3]);
	  off += adv_dio_write_int32(doc, off, n[ 6]);
	  off += adv_dio_write_int32(doc, off, n[ 8]);
	  off += adv_dio_write_int32(doc, off, n[ 9]);
	  off += adv_dio_write_int32(doc, off, n[29]);
	  off += adv_dio_write_int32(doc, off, n[28]);
	  off += adv_dio_write_int32(doc, off, n[30]);
	  off += adv_dio_write_int32(doc, off, n[31]);
	  off += adv_dio_write_int32(doc, off, n[33]);
	  off += adv_dio_write_int32(doc, off, n[32]);

	  off += adv_dio_write_int32(doc, off, n[ 6]);
	  off += adv_dio_write_int32(doc, off, n[ 5]);
	  off += adv_dio_write_int32(doc, off, n[ 8]);
	  off += adv_dio_write_int32(doc, off, n[ 9]);
	  off += adv_dio_write_int32(doc, off, n[14]);
	  off += adv_dio_write_int32(doc, off, n[31]);
	  off += adv_dio_write_int32(doc, off, n[32]);
	  off += adv_dio_write_int32(doc, off, n[26]);
	  off += adv_dio_write_int32(doc, off, n[33]);
	  off += adv_dio_write_int32(doc, off, n[34]);

	  off += adv_dio_write_int32(doc, off, n[ 6]);
	  off += adv_dio_write_int32(doc, off, n[ 5]);
	  off += adv_dio_write_int32(doc, off, n[ 9]);
	  off += adv_dio_write_int32(doc, off, n[ 4]);
	  off += adv_dio_write_int32(doc, off, n[14]);
	  off += adv_dio_write_int32(doc, off, n[32]);
	  off += adv_dio_write_int32(doc, off, n[15]);
	  off += adv_dio_write_int32(doc, off, n[34]);
	  off += adv_dio_write_int32(doc, off, n[20]);
	  off += adv_dio_write_int32(doc, off, n[13]);

	  off += adv_dio_write_int32(doc, off, n[ 5]);
	  off += adv_dio_write_int32(doc, off, n[ 7]);
	  off += adv_dio_write_int32(doc, off, n[ 8]);
	  off += adv_dio_write_int32(doc, off, n[ 9]);
	  off += adv_dio_write_int32(doc, off, n[25]);
	  off += adv_dio_write_int32(doc, off, n[26]);
	  off += adv_dio_write_int32(doc, off, n[34]);
	  off += adv_dio_write_int32(doc, off, n[27]);
	  off += adv_dio_write_int32(doc, off, n[33]);
	  off += adv_dio_write_int32(doc, off, n[21]);

	  off += adv_dio_write_int32(doc, off, n[ 5]);
	  off += adv_dio_write_int32(doc, off, n[ 4]);
	  off += adv_dio_write_int32(doc, off, n[ 7]);
	  off += adv_dio_write_int32(doc, off, n[ 9]);
	  off += adv_dio_write_int32(doc, off, n[13]);
	  off += adv_dio_write_int32(doc, off, n[25]);
	  off += adv_dio_write_int32(doc, off, n[34]);
	  off += adv_dio_write_int32(doc, off, n[19]);
	  off += adv_dio_write_int32(doc, off, n[21]);
	  off += adv_dio_write_int32(doc, off, n[20]);

	  if (mesh->num_elems < 100 || idx % (mesh->num_elems/100) == 0)
	    status_message("dumping elems... %d/%d\n", idx, mesh->num_elems);
	}
      status_message("dumping elems... done\n\n");
    }
}

//============================================================

class IFNodeGen
{
public:
  static IFNodeGen* Create(AdvDatabox* db, Mesh* m, NodeGen* ng);
  static void Destroy(IFNodeGen* ifgen);

  void DumpIFNodes(AdvDocument* doc);

protected:
  IFNodeGen(AdvDatabox* db, Mesh* m, NodeGen* ng);
  virtual ~IFNodeGen();

  Mesh* mesh;
  NodeGen* ngen;

  int32 num_items;
  index_t* sdom_pid;
  index_t* num_snode;
  index_t* num_snodeinfo;
  index_t** snodes;
  index_t** snodeinfo;
};

IFNodeGen*
IFNodeGen::Create(AdvDatabox* db, Mesh* m, NodeGen* ng)
{
  IFNodeGen* ifgen = NULL;
  AdvDocument* doc[2];
  
  doc[0] = adv_dbox_find_by_property(db, NULL,
				     "content_type", "SubDomain",
				     NULL);
  doc[1] = adv_dbox_find_by_property(db, NULL,
				     "content_type", "InterfaceNode",
				     NULL);
  if (doc[0] && doc[1])
    ifgen = new IFNodeGen(db, m, ng);
  return ifgen;
}

void
IFNodeGen::Destroy(IFNodeGen* ifgen)
{ delete ifgen; }

IFNodeGen::IFNodeGen(AdvDatabox* db, Mesh* m, NodeGen* ng)
  : ngen(ng)
  , mesh(m)
{
  AdvDocument* doc;
  doc = adv_dbox_find_by_property(db, NULL,
				  "content_type", "SubDomain",
				  NULL);
  int mypid;
  adv_dio_get_property_int32(doc, "subdomain_number", &mypid);
  
  doc = adv_dbox_find_by_property(db, NULL,
				  "content_type", "InterfaceNode",
				  NULL);

  adv_dio_get_property_int32(doc, "num_items", &num_items);
  sdom_pid  = new index_t[num_items];
  num_snode = new index_t[num_items];
  num_snodeinfo = new index_t[num_items];
  snodes    = new (index_t*)[num_items];
  snodeinfo = new (index_t*)[num_items];
  adv_off_t off = 0;
  for (int i = 0; i < num_items; i++)
    {
      int32 pid, num;
      off += adv_dio_read_int32(doc, off, &pid);
      off += adv_dio_read_int32(doc, off, &num);
      sdom_pid[i] = pid;
      snodes[i] = new index_t[num];
      num_snode[i] = num;
      for (int j = 0; j < num; j++)
	{
	  int32 idx;
	  off += adv_dio_read_int32(doc, off, &idx);
	  snodes[i][j] = idx;
	}
    }
  for (int i = 0; i < num_items; i++)
    {
      AdvDocument* sndoc;
      if (sndoc = adv_dbox_find_by_property(db, NULL,
					    "content_type", "interfaceNodeInfo",
					    "subdomain_number", form("%d", sdom_pid[i]),
					    NULL))
	{
	  int32 num_items;
	  adv_dio_get_property_int32(sndoc, "num_items", &num_items);
	  
	  adv_off_t off = 0;
	  int pid, num;
	  snodeinfo[i] = NULL;
	  for (int j = 0; j < num_items; j++)
	    {
	      off += adv_dio_read_int32(sndoc, off, &pid);
	      off += adv_dio_read_int32(sndoc, off, &num);
	      if (pid == mypid)
		{
		  //printf("--- %d ---\n", sdom_pid[i]);
		  num_snodeinfo[i] = num;
		  snodeinfo[i] = new index_t[num*2];
		  for (int k = 0; k < num; k++)
		    {
		      off += sizeof(int32);
		      off += adv_dio_read_int32(sndoc, off, snodeinfo[i]+k*2);
		      off += adv_dio_read_int32(sndoc, off, snodeinfo[i]+k*2+1);
		      //printf("%d %d\n", snodeinfo[i][k*2], snodeinfo[i][k*2+1]);
		    }
		}
	      else
		off += sizeof(int32)*3*num;
	    }
	  if (snodeinfo[i] == NULL)
	    {
	      printf("Cannot find interface node info between %d and %d\n", mypid, i);
	      printf("interface node info about %d was find but didn't contain about %d\n",
		     i, mypid);
	      abort();
	    }
	}
      else
	{
	  printf("Cannot find interface node info between %d and %d\n", mypid, i);
	  abort();
	}
    }
}

IFNodeGen::~IFNodeGen()
{
  for (int i = 0; i < num_items; i++)
    {
      delete[] snodes[i];
      delete[] snodeinfo[i];
    }
  delete[] snodes;
  delete[] snodeinfo;
  delete[] num_snode;
  delete[] num_snodeinfo;
  delete[] sdom_pid;
}

void
IFNodeGen::DumpIFNodes(AdvDocument* doc)
{
  adv_off_t off = 0;
  adv_dio_set_property(doc, "content_type", "InterfaceNode");
  adv_dio_set_property_int32(doc, "num_items", num_items);
  for (int i = 0; i < num_items; i++)
    {
      status_message("dumping shared nodes... %d (%d/%d)\n",
		     sdom_pid[i], i+1, num_items);
      list<index_t> ilist;
      for (int j = 0; j < num_snode[i]; j++)
	ilist.push_back(snodes[i][j]);
      for (int j = 0; j < num_snode[i]-1; j++)
	for (int k = j+1; k < num_snode[i]; k++)
	  {
	    try
	      {
		index_t idx = ngen->EdgeCenter(snodes[i][j], snodes[i][k]);
		int l;
		for (l = 0; l < num_snodeinfo[i]; l++)
		  {
		    if (snodeinfo[i][l*2] == j && snodeinfo[i][l*2+1] == k)
		      {
			ilist.push_back(idx);
			break;
		      }
		  }
		if (l == num_snodeinfo[i])
		  ;//printf("Hit\n");
	      }
	    catch (...)
	      {}
	  }

      off += adv_dio_write_int32(doc, off, sdom_pid[i]);
      off += adv_dio_write_int32(doc, off, ilist.size());

      for (list<index_t>::iterator it = ilist.begin(); it != ilist.end(); it++)
	off += adv_dio_write_int32(doc, off, *it);

#if 0
      {
	char fname[100];
	sprintf(fname, "%s_%d", ff, sdom_pid[i]);
	FILE* fp = fopen(fname, "w");
	for (list<index_t>::iterator it = ilist.begin(); it != ilist.end(); it++)
	  fprintf(fp, "%d ", *it);
	fprintf(fp, "\n");
	fclose(fp);
      }
#endif
      status_message("dumping shared nodes... done\n\n");
    }
}

//============================================================

class IFNodeInfo
{
public:
  static IFNodeInfo* Create(AdvDatabox* db, int pid, Mesh* m, NodeGen* ng);
  static void Destroy(IFNodeInfo* ifinfo);

  void DumpIFNodeInfo(AdvDocument* doc);

protected:
  IFNodeInfo(AdvDocument* doc, int pid, Mesh* m, NodeGen* ng);
  virtual ~IFNodeInfo();

  Mesh* mesh;
  NodeGen* ngen;

  int32 num_items;
  int mypid;
  index_t* sdom_pid;
  index_t* snode_num;
  index_t** snodes;
};

IFNodeInfo*
IFNodeInfo::Create(AdvDatabox* db, int pid, Mesh* m, NodeGen* ng)
{
  AdvDocument* doc = adv_dbox_find_by_property(db, NULL,
					       "content_type", "InterfaceNode", NULL);
  IFNodeInfo* ifgen = NULL;
  if (doc)
    {
      ifgen = new IFNodeInfo(doc, pid, m, ng);
      adv_dio_close(doc);
    }
  return ifgen;
}

void
IFNodeInfo::Destroy(IFNodeInfo* ifinfo)
{ delete ifinfo; }

IFNodeInfo::IFNodeInfo(AdvDocument* doc, int pid, Mesh* m, NodeGen* ng)
  : ngen(ng)
  , mesh(m)
  , mypid(pid)
{
  adv_dio_get_property_int32(doc, "num_items", &num_items);
  sdom_pid = new index_t[num_items];
  snode_num = new index_t[num_items];
  snodes = new (index_t*)[num_items];
  adv_off_t off = 0;
  for (int i = 0; i < num_items; i++)
    {
      int32 pid, num;
      off += adv_dio_read_int32(doc, off, &pid);
      off += adv_dio_read_int32(doc, off, &num);
      sdom_pid[i] = pid;
      snodes[i] = new index_t[num];
      snode_num[i] = num;
      for (int j = 0; j < num; j++)
	{
	  int32 idx;
	  off += adv_dio_read_int32(doc, off, &idx);
	  snodes[i][j] = idx;
	}
    }
}

IFNodeInfo::~IFNodeInfo()
{
  for (int i = 0; i < num_items; i++)
    delete[] snodes[i];
  delete[] snodes;
  delete[] snode_num;
  delete[] sdom_pid;
}

void
IFNodeInfo::DumpIFNodeInfo(AdvDocument* doc)
{
  adv_off_t off = 0;
  adv_dio_set_property(doc, "content_type", "interfaceNodeInfo");
  adv_dio_set_property_int32(doc, "num_items", num_items);
  adv_dio_set_property_int32(doc, "subdomain_number", mypid);
  for (int i = 0; i < num_items; i++)
    {
      status_message("dumping shared node info... %d (%d/%d)\n",
		     sdom_pid[i], i+1, num_items);

      off += adv_dio_write_int32(doc, off, sdom_pid[i]);
#if 0
      off += adv_dio_write_int32(doc, off, snode_num[i]);
      for (int j = 0; j < snode_num[i]; j++)
	off += adv_dio_write_int32(doc, off, snodes[i][j]);
#endif

      list<index_t> ilist;
      list<index_t> alist;
      list<index_t> blist;
      for (int j = 0; j < snode_num[i]-1; j++)
	for (int k = j+1; k < snode_num[i]; k++)
	  {
	    try
	      {
		index_t idx = ngen->EdgeCenter(snodes[i][j], snodes[i][k]);
		ilist.push_back(idx);
		alist.push_back(j);
		blist.push_back(k);
		//alist.push_back(snodes[i][j]);
		//blist.push_back(snodes[i][k]);
	      }
	    catch (...)
	      {}
	  }
      off += adv_dio_write_int32(doc, off, ilist.size());
      list<index_t>::iterator i_itor = ilist.begin();
      list<index_t>::iterator a_itor = alist.begin();
      list<index_t>::iterator b_itor = blist.begin();
      for (int j = 0; j < ilist.size(); j++)
	{
	  off += adv_dio_write_int32(doc, off, *i_itor);
	  off += adv_dio_write_int32(doc, off, *a_itor);
	  off += adv_dio_write_int32(doc, off, *b_itor);
	  i_itor++;
	  a_itor++;
	  b_itor++;
	}
    }
  status_message("dumping shared node info... done\n\n");
}

//============================================================

void usage(const char* name)
{
  printf("%s: <INPUT>... -o <OUTPUT>\n", name);
  printf("  -v,  --verbose           verbose\n");
  printf("  -sn, --shared-node-info  generate shared node info, and APPEND it to output\n");
  exit(2);
}

int
main(int ac, char** av)
{
  adv_query_time(&start_time);
  setupterm(NULL, fileno(stdout), &flags.terminfo_avail);
  flags.verbose = adv_opt_get_flag(&ac, &av, "v", "verbose");
  flags.sninfo  = adv_opt_get_flag(&ac, &av, "sn", "shared-node-info");
  if (adv_opt_get_string(&ac, &av, "o", "output", &flags.outname) == false)
    flags.outname = NULL;
  if (isatty(fileno(stdout)) == 0)
    flags.terminfo_avail = false;

  if (ac < 2 || !flags.outname)
    usage(av[0]);

  AdvDatabox* db;
  db = adv_dbox_new();
  for (int i = 1; i < ac; i++)
    adv_dbox_add(db, av[i]);

  list<string> didlist;
  
  Mesh* mesh = Mesh::Create();
  mesh->ReadElems(db);
  mesh->ReadNodes(db);

  //>>> read and generate mesh
  NodeGen* ngen = NodeGen::Create(mesh);
  
  int num_faces = mesh->num_surfaces+mesh->num_innfaces;
  int num_nodes = mesh->num_nodes;
  int num_elems = mesh->num_elems;
  message("=Original===================================\n");
  message("nodes: %d\n", num_nodes);
  message("elems: %d\n", num_elems);
  message("\n");

  //<<< read and generate mesh


  AdvDocument* doc;

  if (!flags.sninfo)
    {
      int num_pnodes = ngen->NumNodes();

      message("=Product===================================\n");
      message("nodes: %d\n", ngen->NumNodes());
      message("elems: %d\n", mesh->num_elems*8);
      message("\n");

      AdvDocFile* dfile = adv_dio_file_open(flags.outname, "c");

      //>>> dump mesh
      doc = adv_dio_create(dfile, adv_dio_make_documentid("Elem"));
      didlist.push_back(adv_dio_get_documentid(doc));
      dump_elem(doc, mesh, ngen);
      adv_dio_close(doc);

      doc = adv_dio_create(dfile, adv_dio_make_documentid("Node"));
      didlist.push_back(adv_dio_get_documentid(doc));
      ngen->DumpNodes(doc);
      adv_dio_close(doc);
      //<<< dump mesh

      //>>> genarate shared nodes
      IFNodeGen* ifgen = IFNodeGen::Create(db, mesh, ngen);
      if (ifgen)
	{
	  doc = adv_dio_create(dfile, adv_dio_make_documentid("InterfaceNode"));
	  didlist.push_back(adv_dio_get_documentid(doc));
	  ifgen->DumpIFNodes(doc);
	  IFNodeGen::Destroy(ifgen);
	  adv_dio_close(doc);
	}
      //<<< genarate shared nodes

      mesh->DeleteNodes();

      //>>> genarate fega
      int num_docs = adv_dbox_count_by_property(db,
						"content_type", "FEGenericAttribute",
						NULL);
      message("number of conditions: %d\n", num_docs);
      CondGen* condgen = new CondGen(num_nodes, num_pnodes);
      if (doc = adv_dbox_find_by_property(db, NULL,
					  "content_type", "FEGenericAttribute",
					  NULL))
	do 
	  {
	    condgen->ReadDoc(doc);
	    AdvDocument* wdoc = adv_dio_create(dfile, adv_dio_make_documentid("FEGA"));
	    didlist.push_back(adv_dio_get_documentid(wdoc));
	    condgen->DumpCond(wdoc, mesh, ngen);
	    adv_dio_close(wdoc);
	  }  while (doc = adv_dbox_find_by_property(db, doc,
						    "content_type", "FEGenericAttribute",
						    NULL));
      delete condgen;
      //<<< genarate fega
  
      //>>> genarate SubDomain
      if (doc = adv_dbox_find_by_property(db, NULL,
					  "content_type", "SubDomain",
					  NULL))
	{
	  adv_off_t off = 0;
	  AdvDocument* wdoc = adv_dio_create(dfile, adv_dio_make_documentid("SubDomain"));
	  int32 pid;
	  adv_dio_get_property_int32(doc, "subdomain_number", &pid);
	  adv_dio_set_property(wdoc, "content_type", "SubDomain");
	  adv_dio_set_property_int32(wdoc, "num_items", didlist.size());
	  adv_dio_set_property_int32(wdoc, "subdomain_number", pid);
	  for (list<string>::iterator it = didlist.begin();
	       it != didlist.end(); it++)
	    off += adv_dio_write_string(wdoc, off, it->c_str());
	  adv_dio_close(wdoc);
	}
      //<<< genarate SubDomain

      adv_dio_file_close(dfile);
    }
  else
    {
      AdvDocFile* dfile = adv_dio_file_open(flags.outname, "a");

      int32 pid;
      if (doc = adv_dbox_find_by_property(db, NULL,
					  "content_type", "SubDomain",
					  NULL))
	adv_dio_get_property_int32(doc, "subdomain_number", &pid);
      else
	abort();
      IFNodeInfo* ifinfo = IFNodeInfo::Create(db, pid, mesh, ngen);
      if (ifinfo)
	{
	  doc = adv_dio_create(dfile, adv_dio_make_documentid("IfNInfo"));
	  ifinfo->DumpIFNodeInfo(doc);
	  IFNodeInfo::Destroy(ifinfo);
	  adv_dio_close(doc);
	}

      adv_dio_file_close(dfile);
    }

  adv_dbox_close(db);

  NodeGen::Destroy(ngen);

  mesh->Unref();

  //a_memory_profile();
}
