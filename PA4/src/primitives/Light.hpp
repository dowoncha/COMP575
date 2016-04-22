/******************************************************************************
 *
 *  filename: Light.h
 *  author  : Do Won Cha
 *
 *****************************************************************************/

#pragma once

#ifndef _RAY_LIGHT_
#define _RAY_LIGHT_

#include <Eigen/Core>
#include "primitives/Surface.hpp"

using namespace Eigen;

class Light : public Node
{
public:
  Vecotr3f pos;
  Vector3f amb, diff;
  float intensity;
public:
  Light() : Node() { }

  Light(Vector3f position,
        Vector3f ambient,
        Vector3f diffuse) :
    Node(),
    pos(position)
    amb(ambient),
    diff(diffuse),
    intensity(1.0f)
  {
    Position = pos;
  }

  Light::~Light()
  {
  }

  void intensity(float i)
  {
    intensity = i;
  }
};

#endif //_RAY_LIGHT guard
