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

Camera::~Camera()
{ }

// default offset is to the center of the pixel
Ray Camera::GetRay(int x, int y) const
{
    assert(x < ScreenWidth);
    assert(y < ScreenHeight);

	float invW = 1.0f / ScreenWidth;
	float invH = 1.0f / ScreenHeight;

	float u = l + (r - l) * ((float)x + 0.5f) * invW;
	float v = b + (t - b) * ((float)y + 0.5f) * invH;

    // FIX: Had to change up calculation to negative to set y to bottom?
	Vector3f dir = ((Right * u) - (Up * v) - (Forward * d)).Normalized();

	return Ray(Position, dir);
}

Ray Camera::GetRay(int x, int y, float offsetx, float offsety) const
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

    Vector3f dir = ((Right * u) - (Up * v) - (Forward * d)).Normalized();

	return Ray(Position, dir);
}

void Camera::SetScreenSize(int width, int height)
{
    ScreenWidth = width;
    ScreenHeight = height;
}
