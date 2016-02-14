#include "Surface.h"

Sphere::Sphere()
{
}

Sphere::Sphere(const Vector3f& center, float radius, Material* mat) :
  Surface(),
  Radius(radius),
  Radius2(radius * radius)
{
  Position = center;
  Mat = mat;
}

bool Sphere::Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const
{
  // Calculate ray-sphere intersection using geometric approach
  Vector3f len = Position - ray.Position;
  float s = len * ray.Direction;
  float len2 = len.Length() * len.Length();

  if (s > 0.0f)
  {
    float m2 = len2 - s * s;

    if (m2 < Radius2)
    {
      float q = std::sqrt(Radius2 - m2);

      t = (len2 > Radius2) ? s - q : s + q;

      Point = ray.Evaluate(t);

      return (t > 0.0f && tMax);
    }
  }

  return false;
}

bool Sphere::Intersect(const Ray& ray, float tMax, float& t) const
{
  // Calculate ray-sphere intersection using geometric approach
  Vector3f len = Position - ray.Position;
  float s = len * ray.Direction;
  float len2 = len.Length() * len.Length();

  if (s > 0.0f)
  {
    float m2 = len2 - s * s;

    if (m2 < Radius2)
    {
      float q = std::sqrt(Radius2 - m2);

      t = (len2 > Radius2) ? s - q : s + q;

      return (t > 0.0f && t < tMax);
    }
  }

  return false;
}

bool Sphere::Intersect(const Ray& ray, float tMax) const
{
  // Calculate ray-sphere intersection using geometric approach
  Vector3f len = Position - ray.Position;
  float s = len * ray.Direction;
  float len2 = len.Length() * len.Length();

  if (s > 0.0f)
  {
    float m2 = len2 - s * s;

    if (m2 < Radius2)
    {
      float q = std::sqrt(Radius2 - m2);
      float t = (len2 > Radius2) ? s - q : s + q;

      return (t > 0.0f && t < tMax);
    }
  }

  return false;
}

Vector3f Sphere::GetNormal(const Vector3f& Point) const
{
  return (Point - Position).Normalized();
}

Plane::Plane()
{
}

Plane::Plane(const Vector3f& pos, const Vector3f& normal, Material* mat) :
  Surface(),
  Normal(normal.Normalized())
  {
    Position = pos;
    Mat = mat;
  }

Plane::~Plane() { }

bool Plane::Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const
{
  float denom = Normal * ray.Direction;

  if (std::fabs(denom) > 1e-6)
  {
    t = (Normal * (Position - ray.Position)) / denom;
    if (t >= 0.0f)
    {
      Point = ray.Evaluate(t);
      return true;
    }
  }

  return false;
}

bool Plane::Intersect(const Ray& ray, float tMax, float& t) const
{
  float denom = Normal * ray.Direction;

  if (std::fabs(denom) > 1e-6)
  {
    t = (Normal * (Position - ray.Position)) / denom;
    if (t > 0.0f)
    {
      return true;
    }
  }

  return false;
}

bool Plane::Intersect(const Ray& ray, float tMax) const
{
  float denom = Normal * ray.Direction;

  if (std::fabs(denom) > 1e-6)
  {
    float t = (Normal * (Position - ray.Position)) / denom;
    if (t >= 0.0f)
    {
      return true;
    }
  }

  return false;
}

Vector3f Plane::GetNormal(const Vector3f& p) const
{
  return Normal;
}
