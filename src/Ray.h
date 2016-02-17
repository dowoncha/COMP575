#pragma once

#include "Primitives.h"

class Ray
{
public:
  Ray() { }
  Ray(const Vector3f& position,
      const Vector3f& direction) :
  Position(position),
  Direction(direction)
  { }

  ~Ray() {}

  Vector3f Evaluate(float t) const { return Position + Direction * t; }

  friend std::ostream& operator << (std::ostream& s, const Ray& ray)
  {
	  return s << ray.Position << ' ' << ray.Direction;
  }
public:
  Vector3f Position;
  Vector3f Direction;
};
