/**
 *  filename : surface_plane.h
 *  content  : A plane
 */

#pragma once
#ifndef _RAY_SURFACE_
#define _RAY_SURFACE_

#include <Eigen/Core>
#include <string>
#include "surface.h"
#include "ray.h"

namespace raytracer
{

using namespace Eigen;

class Plane : public Surface
{
public:
  // @param position of the plane
  // @param normal of the plane
  // @param material to apply to plane
  Plane(Vector3f position, Vector3f normal, material_t material) :
    Surface(position, material),
    normal_(normal.normalized())
  {}

  ~Plane()
  { }

  bool Intersect(const Ray& ray, Vector3f& hit_point, Vector3f& hit_normal) const
  {
    // cos a between the normal and ray
    float denom = normal_.dot(ray.direction());

    if (std::fabs(denom) > .0001f)
    {
      // Time of intersection with the plane
      float t = (position_ - ray.position()).dot(normal_) / denom;
      if (t > 0.0001f)
      {
        hit_point = ray.evaluate(t);
        hit_normal = normal();
        return true;
      }
    }

    return false;
  }

  bool Intersect(const Ray& ray) const
  {
    float denom = normal_.dot(ray.direction());

    if (std::fabs(denom) > 1e-6)
    {
      // Time of intersection with the plane
      float plane_hit_time = normal_.dot(position_ - ray.position()) / denom;
      if (plane_hit_time > 0.0f)
      {
        return true;
      }
    }

    return false;
  }

  Vector3f normal() const
  {
    return normal_;
  }
private:
  Vector3f normal_;
};

}   // end of namespace raytracer

#endif // _RAY_SURFACE_
