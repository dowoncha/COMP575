#include "Surfaces.h"

#pragma once

#ifndef _RAY_LIGHT_
#define _RAY_LIGHT_

class Light : public Node
{
public:
  Vector3f Ambient, Diffuse;
  float Intensity;
public:
  Light();
  Light(const Vector3f& pos, const Vector3f& ambient, const Vector3f& diff);
  ~Light();
};

#endif //_RAY_LIGHT guard
