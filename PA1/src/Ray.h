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

#include <glm/vec3.hpp>
//#include <glm/gtx/string_cast.hpp>

class Ray
{
public:
  Ray() { }
  Ray(const glm::vec3& position,
      const glm::vec3& direction) :
  Position(position),
  Direction(direction)
  { }

  ~Ray() {}

  glm::vec3 Evaluate(float t) const { return Position + Direction * t; }

  /*
  friend std::ostream& operator << (std::ostream& s, Ray const & ray)
  {
	  return s << glm::to_string(ray.Position) << ' ' << glm::to_string(ray.Direction);
  }
  */
public:
  glm::vec3 Position;
  glm::vec3 Direction;
};

#endif // _RAY_RAY_
