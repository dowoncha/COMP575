#include "Surfaces.h"

#pragma once

#ifndef _RAY_LIGHT_
#define _RAY_LIGHT_

class Light : public Node
{
public:
  float Ambient, Diffuse;
public:
  Light();
  Light(const Vector3f& pos);
  ~Light();
};

#endif //_RAY_LIGHT guard
