#include "Surface.hpp"

class Sphere : public Surface
{
public:
  Surface(const Material& mat) :
    Node(), mMaterial(mat)
  { }

  Sphere(const Vector3f& center, float radius, const Material& mat) :
    Surface(mat),
    Radius(radius),
    Radius2(radius * radius)
  {
    Position = center;
  }

  ~Sphere()
  {
  }

  bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const
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

  bool Intersect(const Ray& ray, float tMax, float& t) const
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

  bool Intersect(const Ray& ray, float tMax) const
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

  Vector3f GetNormal(const glm::vec3& Point) const
  {
    return (Point - Position).normalized();
  }
private:
  float Radius, Radius2;
};
