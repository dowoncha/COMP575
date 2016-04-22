/******************************************************************************
 *
 *  filename: Ray.h
 *  author  : Do Won Cha
 *  content :
 *
 *****************************************************************************/

#pragma once
#ifndef _RAY_RAY_
#define _RAY_RAY_

#include <Eigen/Core>
#include <iostream>
#include "Ray.hpp"

class Ray
{
public:
  Ray() { }

  Ray(Vector3f position,
      Vector3f direction) :
  pos(position),
  direction(direction)
  { }

  ~Ray() {}

  Vector3f evaluate(float t) const { return Position + Direction * t; }

  friend std::ostream& operator << (std::ostream& s, Ray const & ray)
  {
	  return s << "";
  }

public:
  Vector3f pos;
  Vector3f direction;
};

#endif // _RAY_RAY_
