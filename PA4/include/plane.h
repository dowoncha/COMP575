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
  Plane(const Vector3f& position, const Vector3f& normal, std::string material) :
    Surface(position, material),
    normal_(normal.normalized())
  {}

  bool intersect(const Ray& ray, HitData& hit) const
  {
    // cos a between the normal and ray
    float denom = normal_.dot(ray.direction());

    if (std::fabs(denom) > 1e-4)
    {
      // Time of intersection with the plane
      float t = (position_ - ray.position()).dot(normal_) / denom;
      if (t > 1e-4)
      {
        if (t < hit.t)
        {
          hit.t = t;
          hit.point = ray.evaluate(t);
          hit.normal = normal();
        }
        return true;
      }
    }

    return false;
  }

  bool intersect(const Ray& ray) const
  {
    float denom = normal_.dot(ray.direction());

    if (std::fabs(denom) > 1e-4)
    {
      // Time of intersection with the plane
      float plane_hit_time = normal_.dot(position_ - ray.position()) / denom;
      if (plane_hit_time > 1e-4)
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
