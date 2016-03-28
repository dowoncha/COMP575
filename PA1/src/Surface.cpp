#include "Surface.h"

Surface::Surface(const Material& mat) :
  Node(), mMaterial(mat)
{ }

Sphere::Sphere(const glm::vec3& center, float radius, const Material& mat) :
  Surface(mat),
  Radius(radius),
  Radius2(radius * radius)
{
  Position = center;
}

Sphere::~Sphere()
{
}

bool Sphere::Intersect(const Ray& ray, float tMax, float& t, glm::vec3& Point) const
{
  // Calculate ray-sphere intersection using geometric approach
  glm::vec3 len = Position - ray.Position;
  float s = glm::dot(len, ray.Direction);
  float len2 = glm::length(len) * glm::length(len);

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
  glm::vec3 len = Position - ray.Position;
  float s = glm::dot(len, ray.Direction);
  float len2 = glm::length(len) * glm::length(len);

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
  glm::vec3 len = Position - ray.Position;
  float s = glm::dot(len, ray.Direction);
  float len2 = glm::length(len) * glm::length(len);

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

glm::vec3 Sphere::GetNormal(const glm::vec3& Point) const
{
  return glm::normalize(Point - Position);
}

Plane::Plane(const glm::vec3& pos, const glm::vec3& normal, const Material& mat) :
  Surface(mat),
  Normal(glm::normalize(normal))
{
  Position = pos;
}

Plane::~Plane()
{ }

bool Plane::Intersect(const Ray& ray, float tMax, float& t, glm::vec3& Point) const
{
  float denom = glm::dot(Normal, ray.Direction);

  if (std::fabs(denom) > 1e-6)
  {
    t = glm::dot(Normal, Position - ray.Position) / denom;
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
  float denom = glm::dot(Normal, ray.Direction);

  if (std::fabs(denom) > 1e-6)
  {
    t = glm::dot(Normal, (Position - ray.Position)) / denom;
    if (t > 0.0f)
    {
      return true;
    }
  }

  return false;
}

bool Plane::Intersect(const Ray& ray, float tMax) const
{
  float denom = glm::dot(Normal, ray.Direction);

  if (std::fabs(denom) > 1e-6)
  {
    float t = glm::dot(Normal, (Position - ray.Position)) / denom;
    if (t >= 0.0f)
    {
      return true;
    }
  }

  return false;
}

glm::vec3 Plane::GetNormal() const
{
  return Normal;
}

glm::vec3 Plane::GetNormal(const glm::vec3& p) const
{
  return Normal;
}
