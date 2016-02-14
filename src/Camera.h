#include "Primitives.h"
#include "Ray.h"

class Camera
{
public:
  Camera();

  Camera(const Vector3f& position,
         const Vector3f& target);

  Camera(const Vector3f& position,
         const Vector3f& target,
         const Vector3f& up);
  ~Camera() {}

  void SetScreenSize(int width, int height);

  Ray GetRay(int x, int y) const;
public:
  Vector3f Position;
  Vector3f Target;
  Vector3f Forward;
  Vector3f Up;
  Vector3f Right;
  float l, r, t, b, d;
  int ScreenWidth, ScreenHeight;
};
