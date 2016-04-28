#include "camera.h"

using namespace raytracer;

Camera::Camera() :
  position_  (0.0f, 0.0f, 0.0f),
  right_     (1.0f, 0.0f, 0.0f),
  up_        (0.0f, 1.0f, 0.0f),
  forward_   (0.0f, 0.0f, 1.0f),
  l(-0.1f), r(0.1f), t(0.1f), b(-0.1f), d(0.1f),
  screen_width_(512),
  screen_height_(512)
{
}

Camera::Camera(int width, int height) :
  position_  (0.0f, 0.0f, 0.0f),
  right_     (1.0f, 0.0f, 0.0f),
  up_        (0.0f, 1.0f, 0.0f),
  forward_   (0.0f, 0.0f, 1.0f),
  l(-0.1f), r(0.1f), t(0.1f), b(-0.1f), d(0.1f),
  screen_width_(width),
  screen_height_(height)
{}

// default offset is to the center of the pixel
Ray Camera::GetRayFromEye(int x, int y) const
{	
  assert(x < screen_width_);
  assert(y < screen_height_);

	float invW = 1.0f / screen_width_;
	float invH = 1.0f / screen_height_;

	float u = l + (r - l) * ((float)x + 0.5f) * invW;
	float v = b + (t - b) * ((float)y + 0.5f) * invH;

  // NOTE: Had to change up calculation to negative to set y to bottom?
  // forward_ is also negative
	Vector3f dir = ((right_ * u) + (up_ * v) - (forward_ * d)).normalized();

	return Ray(position_, dir);
}

void Camera::resize(int width, int height)
{
  screen_width_ = width;
  screen_height_ = height;
}
