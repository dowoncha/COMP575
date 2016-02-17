/**
 *
 * 	filename   : Camera.h
 *  author     : Do Won Cha
 *  content    : Container class to output a buffer to image.
 *
 */

#include <cassert>

#include "Primitives.h"
#include "Ray.h"
#include "Utility.h"

class Camera
{
public:
  Camera();

  Camera(const Vector3f& position,
         const Vector3f& target);

  Camera(const Vector3f& position,
         const Vector3f& target,
         const Vector3f& up);
  ~Camera();

  void SetScreenSize(int width, int height);

  // Have 2 just in case, both work exactly the same way, maybe i should make these templates.
  //template<typename T>
  //Ray GetRay(T x, T y) const;

  Ray GetRay(int x, int y) const;
  Ray GetRay(int x, int y, float offsetx, float offsety) const;
public:
  Vector3f Position;
  Vector3f Target;
  Vector3f Forward;
  Vector3f Up;
  Vector3f Right;
  float l, r, t, b, d;
  int ScreenWidth, ScreenHeight;
};
