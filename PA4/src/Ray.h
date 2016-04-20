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

class Ray
{
public:
  Ray() { }
  Ray(Vector3f position,
      Vector3f direction) :
  Position(position),
  Direction(direction)
  { }

  ~Ray() {}

  Vector3f Evaluate(float t) const { return Position + Direction * t; }

  friend std::ostream& operator << (std::ostream& s, Ray const & ray)
  {
	  return s << "";
  }

public:
  Vector3f Position;
  Vector3f Direction;
};

#endif // _RAY_RAY_
