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
  Sphere(const Vector3f& center, float radius, const std::string& material) :
    Surface(center, material),
    radius2_(radius * radius)
  {
  }

  virtual ~Sphere()
  {}

  bool intersect(const Ray& ray, HitData& hit) const
  {
    // NOTE: this should have been normalized.
    // Calculate ray-sphere intersection using geometric approach
    Vector3f ray_to_center = (position_ - ray.position()).normalized();
    // Cos theta of hit point and ray
    float s = ray_to_center.dot(ray.direction());
    // Angle of ray misses sphere
    //if (s < 0.0f) return false;
    float m2 = ray_to_center.dot(ray_to_center) - s * s;

    if (m2 > radius2_) return false;

    float thc = std::sqrt(radius2_ - m2);

    float t0 = s - thc;
    float t1 = s + thc;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0) return false; // both t0 and t1 are negative
    }

    t = t0;
    if (t < hit.t)
    {
      hit.t = t;
      hit.point = ray.evaluate(t);
      hit.normal = normal();
    }

    return true;
  }

  bool Intersect(const Ray& ray) const
  {
    // NOTE: this should have been normalized.
    // Calculate ray-sphere intersection using geometric approach
    Vector3f ray_to_center = (position_ - ray.position()).normalized();
    // Cos theta of hit point and ray
    float s = ray_to_center.dot(ray.direction());
    // Angle of ray misses sphere
    //if (s < 0.0f) return false;
    float m2 = ray_to_center.dot(ray_to_center) - s * s;

    if (m2 > radius2_) return false;

    float thc = std::sqrt(radius2_ - m2);

    float t0 = s - thc;
    float t1 = s + thc;

    if (t0 > t1) std::swap(t0, t1);

    if (t0 < 0) {
        t0 = t1; // if t0 is negative, let's use t1 instead
        if (t0 < 0) return false; // both t0 and t1 are negative
    }

    return true;
  }

  Vector3f normal(const Vector3f& point) const
  {
    return (point - position_).normalized();
  }
private:
  float radius2_;
};

}

#endif /* end of include guard: _RAY_SPHERE_ */
