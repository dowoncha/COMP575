#pragma once
#ifndef _RAY_SPHERE_
#define _RAY_SPHERE_

#include <Eigen/Core>
#include "ray.h"
#include "surface.h"
#include "material.h"

using namespace Eigen;

namespace raytracer
{

class Sphere : public Surface
{
public:
  Sphere(Vector3f center, float radius, material_t material) :
    Surface(center, material),
    radius_(radius),
    radius2_(radius * radius)
  {
  }

  ~Sphere()
  {
  }

  bool Intersect(const Ray& ray, Vector3f& hit_point, Vector3f& hit_normal) const
  {
    // Calculate ray-sphere intersection using geometric approach
    Vector3f posray = position_ - ray.position();
    // Cos theta of hit point and ray
    float s = posray.dot(ray.direction());
    float length2 = posray.squaredNorm();

    // If the angle between the ray and the direction is less than 90
    if (s > 0.0f)
    {
      float m2 = length2 - s * s;

      if (m2 < radius2_)
      {
        float q = std::sqrt(radius2_ - m2);

        float t = (length2 > radius2_) ? s - q : s + q;
        hit_point = ray.evaluate(t);
        hit_normal = normal(hit_point);

        return (t > 0.0f);
      }
    }

    return false;
  }

  bool Intersect(const Ray& ray) const
  {
    // Calculate ray-sphere intersection using geometric approach
    Vector3f posray = position_ - ray.position();
    // Cos theta of hit point and ray
    float s = posray.dot(ray.direction());
    float length2 = posray.squaredNorm();

    // If the angle between the ray and the direction is less than 90
    if (s > 0.0f)
    {
      float m2 = length2 - s * s;

      if (m2 < radius2_)
      {
        float q = std::sqrt(radius2_ - m2);

        float t = (length2 > radius2_) ? s - q : s + q;
        return (t > 0.0f);
      }
    }

    return false;
  }

  Vector3f normal(const Vector3f& point) const
  {
    return (point - position_).normalized();
  }
private:
  float radius_, radius2_;
};

}

#endif /* end of include guard: _RAY_SPHERE_ */
