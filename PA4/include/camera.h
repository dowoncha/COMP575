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

#include <Eigen/Core>
#include <cassert>

#include "ray.h"
#include "../utility.h"

namespace raytracer
{

using namespace Eigen;

class Camera
{
public:
  Camera();

  Camera(int width, int height);

  // default offset is to the center of the pixel
  Ray getRay(int x, int y) const;

  // Get ray with a float offset.
  Ray getRay(int x, int y, float offsetx, float offsety) const;

  void resize(int width, int height);

  int screen_width()  const { return screen_width_; }
  int screen_height() const { return screen_height_; }

  void set_position(const Vector3f& position) { position_ = position; }
  void set_u(const Vector3f& u) { right_ = u; }
  void set_v(const Vector3f& v) { up_ = v; }
  void set_w(const Vector3f& w) { forward_ = w; }
private:
  Vector3f position_;                // position vector
  Vector3f forward_;                 // forward_ vector
  Vector3f up_;                      // up_ vector, typically y-axis
  Vector3f right_;                   // right_ vector
  float l, r, t, b, d;              // Viewport constants
  int screen_width_, screen_height_;    // Resolution of camera
};

}     // end of namespace raytracer

#endif // _RAY_CAMERA_
