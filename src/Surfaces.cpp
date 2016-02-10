#include "Surfaces.h"

Sphere::Sphere()
{
}

Sphere::Sphere(const Vector3f& center, float radius, Material* mat) :
  Surface(),
  Center(center),
  Radius(radius),
  Radius2(radius * radius),
  Mat(mat)
{
}

bool Sphere::Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const
{
  // Calculate ray-sphere intersection using geometric approach
  Vector3f len = Center - ray.Position;
  float s = len * ray.Direction.Normalized();
  float len2 = len.Length() * len.Length();

  if (s > 0.0f)
  {
    float m2 = len2 - s * s;

    if (m2 < Radius2)
    {
      float q = std::sqrt(Radius2 - m2);

      if (len2 > Radius2) t = s - q;
      else t = s + q;

      std::cout << "intersected!\n";
      Point = ray.Evaluate(t);
      return true;
      }
  }

  return false;
}

Plane::Plane(Vector3f point, Vector3f normal) :
  Surface(),
  Point(point),
  Normal(normal)
  { }

Plane::~Plane() { }

bool Plane::Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const
{
  //float denom = Normal * ray.Normalize();

  //if (denom < 1e-6) return false;

  //t = ( a - p ) * n / (d * n);
  return true;
}
