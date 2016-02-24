/******************************************************************************
 *
 * 	filename   : Camera.h
 *  author     : Do Won Cha
 *  content    : Container class to output a buffer to image.
 *
 ******************************************************************************/

#pragma once
#ifndef _RAY_CAMERA_
#define _RAY_CAMERA_

#include <cassert>

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "Ray.h"
#include "Utility.h"

class Camera
{
public:
  Camera();

  Camera(glm::vec3 const & position,
         glm::vec3 const & target);

  Camera(glm::vec3 const & position,
         glm::vec3 const & target,
         glm::vec3 const & up);

  ~Camera();

  void SetScreenSize(int width, int height);

  // Make a ray from the x, y coordinate passed
  Ray GetRay(int x, int y) const;
  Ray GetRay(int x, int y, float offsetx, float offsety) const;
public:
  glm::vec3 Position;
  glm::vec3 Target;
  glm::vec3 Forward;
  glm::vec3 Up;
  glm::vec3 Right;
  float l, r, t, b, d;
  int ScreenWidth, ScreenHeight;
};

#endif // _RAY_CAMERA_
