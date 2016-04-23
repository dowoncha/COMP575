#pragma once
#ifndef _RAY_SURFACE_
#define _RAY_SURFACE_

#include <Eigen/Core>
#include "surface.hpp"
#include "ray.hpp"

namespace raytracer
{

using namespace Eigen;

class Plane : public Surface
{
public:

  // @param position of the plane
  // @param normal of the plane
  Plane(Vector3f position, Vector3f normal, std::string material_name = "") :
    Surface(position, material_name),
    normal_(normal.normalized())
  {
  }

  ~Plane()
  { }

  bool Intersect(const Ray& ray, HitData& hit) override
  {
    float denom = normal_.dot(ray.direction());

    if (std::fabs(denom) > 1e-6)
    {
      // Time of intersection with the plane
      float planeHitTime = normal_.dot(position_ - ray.position()) / denom;
      if (planeHitTime >= 0.0f)
      {
        hit.hit_surface = dynamic_cast<Surface*>(this);
        hit.hit_point = ray.evaluate(hit.t);
        hit.t = planeHitTime;
        return true;
      }
    }

    return false;
  }

  Vector3f normal() const override
  {
    return normal_;
  }
private:
  Vector3f normal_;
};

}   // end of namespace raytracer

#endif // _RAY_SURFACE_
