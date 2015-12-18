/***********************************************************
Copyright (C) 2000,2001,2002,2006
Shinobu Yoshimura, University of Tokyo,
the Japan Society for the Promotion of Science (JSPS)
All Rights Reserved
***********************************************************/

#ifndef _8bizer_h_
#define _8bizer_h_

#include <Adv/AdvFileIO.h>

typedef int32 index_t;

struct Vector3D
{
  float32 x, y, z;
  Vector3D()
    : x(0), y(0), z(0) {}
  Vector3D(float32 a, float32 b, float32 c)
    : x(a), y(b), z(c) {}
};
typedef Vector3D Node;

void message(const char* fmt, ...);
void status_message(const char* fmt, ...);

#endif

