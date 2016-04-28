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

namespace raytracer
{

using namespace Eigen;

class Ray
{
public:
  Ray() { }

  Ray(Vector3f position,
      Vector3f direction) :
  position_(position),
  direction_(direction.normalized())
  { }

  ~Ray() {}

  Vector3f evaluate(float t) const { return position_ + direction_ * t; }

  Vector3f position() const { return position_; }
  Vector3f direction() const { return direction_; }

  friend std::ostream& operator << (std::ostream& s, const Ray& ray)
  {
	  return s << "";
  }

private:
  Vector3f position_;
  Vector3f direction_;
};

}     // end of namespace raytracer

#endif // _RAY_RAY_
