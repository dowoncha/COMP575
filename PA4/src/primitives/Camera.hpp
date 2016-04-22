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
#include <Eigen/Core>

#include "Ray.hpp"
#include "Utility.h"

using namespace Eigen;

class Camera
{
public:
  Camera() :
    Position  (0.0f, 0.0f, 0.0f),
    Target    (0.0f, 0.0f, -1.0f),
    Right     (1.0f, 0.0f, 0.0f),
    Up        (0.0f, 1.0f, 0.0f),
    Forward   (0.0f, 0.0f, 1.0f),
    l(-0.1f), r(0.1f), t(0.1f), b(-0.1f), d(0.1f)
  {
  }

  Camera(int width, int height) :
    Position  (0.0f, 0.0f, 0.0f),
    Target    (0.0f, 0.0f, -1.0f),
    Right     (1.0f, 0.0f, 0.0f),
    Up        (0.0f, 1.0f, 0.0f),
    Forward   (0.0f, 0.0f, 1.0f),
    l(-0.1f), r(0.1f), t(0.1f), b(-0.1f), d(0.1f),
    ScreenWidth(width),
    ScreenHeight(height)
  {}

  Camera(Vector3f position,
         Vector3f target) :
    Position(position),
    Target(target),
    Forward(Position - Target),
    Up(0.0f, 1.0f, 0.0f),
    l(-0.1f), r(0.1f), t(0.1f), b(-0.1f), d(0.1f)
  { }

  Camera(Vector3f position,
         Vector3f target,
         Vector3f up) :
    Position(position),
    Target(target),
    Forward(Position - Target),
    Up(up),
    l(-0.1f), r(0.1f), t(0.1f), b(-0.1f), d(0.1f)
  { }

  Camera::~Camera()
  { }

  // default offset is to the center of the pixel
  Ray getRay(int x, int y) const
  {
    assert(x < ScreenWidth);
    assert(y < ScreenHeight);

  	float invW = 1.0f / ScreenWidth;
  	float invH = 1.0f / ScreenHeight;

  	float u = l + (r - l) * ((float)x + 0.5f) * invW;
  	float v = b + (t - b) * ((float)y + 0.5f) * invH;

    // NOTE: Had to change up calculation to negative to set y to bottom?
    // Forward is also negative
  	Vector3f dir = ((Right * u) - (Up * v) - (Forward * d)).normalized());

  	return Ray(Position, dir);
  }

  Ray getRay(int x, int y, float offsetx, float offsety) const
  {
    assert( x < ScreenWidth );
    assert( y < ScreenHeight );
    assert( offsetx <= 1.0f);
    assert( offsety <= 1.0f);

    offsetx = Utility::clamp(0.0f, offsetx, 1.0f);
    offsety = Utility::clamp(0.0f, offsety, 1.0f);

    float invW = 1.0f / ScreenWidth;
    float invH = 1.0f / ScreenHeight;

    float u = l + (r - l) * (x + offsetx) * invW;
  	float v = b + (t - b) * (y + offsety) * invH;

    Vector3f dir = ((Right * u) - (Up * v) - (Forward * d)).normalized();

  	return Ray(Position, dir);
  }

  void resize(int width, int height)
  {
      ScreenWidth = width;
      ScreenHeight = height;
  }
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
