/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#ifndef _mesh_h_
#define _mesh_h_

#include <Adv/AdvDocument.h>
#include <string>
#include <list>
#include <vector>
#include "m8bizer.h"

class BCondGroup;

struct Mesh
{
public:
  static Mesh* Create();
  void Ref();
  void Unref();

  void ReadData(AdvDatabox* adb);

  bool ReadNodes(AdvDatabox* adb);
  bool ReadElems(AdvDatabox* adb);

  void DeleteNodes();				  // delete nodes buffer
  void DeleteElems();				  // delete elems buffer
  void DeleteFaces();				  // delete faces buffer

  index_t num_nodes;
  Node* nodes;					  // original coordinate of nodes

  string element_type;
  index_t num_elems;
  index_t num_nodes_per_elem;
  index_t num_faces_per_elem;
  index_t num_nodes_per_face;
  index_t* elems;

  index_t num_surfaces;
  index_t num_innfaces;
  index_t* faces;				  // faces
						  // surfaces besids first, innerfaces follow after
						  // total size is num_surfaces+num_innfaces

  // documentid
  string nodes_did;
  string elems_did;
  string values_did;

protected:
  // [cd]tor called by Create() & Unrerf()
  Mesh();
  ~Mesh();

  bool ReadNodes(AdvDocument* doc);
  bool ReadElems(AdvDocument* doc);
  bool ReadBCs(AdvDocument* doc);

  void ExtractSurfaces();

private:
  int ref_count;
};

#endif // _mesh_h_
