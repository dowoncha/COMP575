#include "Camera.h"

Camera::Camera() :
  Position  (0.0f, 0.0f, 0.0f),
  Target    (0.0f, 0.0f, -1.0f),
  Right     (1.0f, 0.0f, 0.0f),
  Up        (0.0f, 1.0f, 0.0f),
  Forward   (0.0f, 0.0f, 1.0f),
  l(-0.1f), r(0.1f), t(0.1f), b(-0.1f), d(0.1f)
{
}

Camera::Camera(const Vector3f& position,
               const Vector3f& target) :
  Position(position),
  Target(target),
  Forward(Position - Target),
  Up(0.0f, 1.0f, 0.0f),
  l(-0.1f), r(0.1f), t(0.1f), b(-0.1f), d(0.1f)
  { }

Camera::Camera(const Vector3f& position,
               const Vector3f& target,
               const Vector3f& up) :
  Position(position),
  Target(target),
  Forward(Position - Target),
  Up(up),
  l(-0.1f), r(0.1f), t(0.1f), b(-0.1f), d(0.1f)
  { }

Ray Camera::GetRay(int x, int y)
{
	float invW = 1.0f / ScreenWidth;
	float invH = 1.0f / ScreenHeight;

	float u = l + (r - l) * ((float)(x + 0.5f)) * invW;
	float v = b + (t - b) * ((float)(y + 0.5f)) * invH;

	Vector3f dist = ((Right * u) + (Up * v) - (Forward * d));
  dist.Normalize();
	return Ray(Position, dist);
}

void Camera::SetScreenSize(int width, int height)
{
  ScreenWidth = width;
  ScreenHeight = height;
}
