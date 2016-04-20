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

  Camera(const Vector3f& position,
         Vector3f const & target);

  Camera(Vector3f const & position,
         Vector3f const & target,
         Vector3f const & up);

  ~Camera();

  void SetScreenSize(int width, int height);

  // Make a ray from the x, y coordinate passed
  Ray GetRay(int x, int y) const;

  Ray GetRay(int x, int y, float offsetx, float offsety) const;
public:
  Vector3f Position;                // Position vector
  Vector3f Target;                  // What are we looking at
  Vector3f Forward;                 // Forward vector
  Vector3f Up;                      // Up vector, typically y-axis
  Vector3f Right;                   // Right vector
  float l, r, t, b, d;              // Viewport constants
  int ScreenWidth, ScreenHeight;    // Resolution of camera
};

#endif // _RAY_CAMERA_
