/******************************************************************************
 *
 *  filename: Light.h
 *  author  : Do Won Cha
 *
 *****************************************************************************/

#pragma once

#ifndef _RAY_LIGHT_
#define _RAY_LIGHT_

#include "Surface.h"

class Light : public Node
{
public:
  Vector3f Ambient, Diffuse;
  float Intensity;
public:
  Light();
  Light(const Vector3f& pos, const Vector3f& ambient, const Vector3f& diff);
  ~Light();

  void SetIntensity(float i);
};

#endif //_RAY_LIGHT guard
