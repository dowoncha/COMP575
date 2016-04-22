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

using namespace Eigen;

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

  Vector3f position()
  {
    return pos;
  }

  Vector3f direction()
  {
    return dir;
  }

  friend std::ostream& operator << (std::ostream& s, Ray const & ray)
  {
	  return s << "";
  }

private:
  Vector3f pos;
  Vector3f dir;
};

#endif // _RAY_RAY_
