#pragma once
#ifndef _RAY_TRIANGLE_
#define _RAY_TRIANGLE_

#include <Eigen/Core>
#include "ray.hpp"
#include "surface.hpp"

namespace raytracer
{

using namespace Eigen;

class Triangle : public Surface
{
public:
  // Triangle indices
  typedef Matrix<unsigned int, 3, 1> Vector3u;

  Triangle(unsigned int a, unsigned int b, unsigned int c, const Surface& owner) :
    indices_(a, b, c),
    owner_(owner);
  {
  }

  bool Intersect(const Ray& ray, HitData& data) override
  {
    // Vertices
    const Vector3f& p0 = owner.vertices_.at(indices_(0));
    const Vector3f& p1 = owner.vertices_.at(indices_(1));
    const Vector3f& p2 = owner.vertices_.at(indices_(2));

    // Calculate the edges
    Vector3f edge1 = p1 - p0;
    Vector3f edge2 = p2 - p0;

    Vector3f q = ray.distance().cross(e2);
    float alpha = edge1.dot(q);             // Determinant of matrix m

    if (alpha > eps && alpha < eps)
    {
      // data.u = 0
      // data.v = 0
      // data.time = 0
      return false;
    }
    float invAlpha = 1.0f / alpha;
    Vector3f source = ray.position() - p0;
    float u = invAlpha * source.dot(q);
    if (u < 0.0f)
    {
      // data.u = 0
      // data.v = 0
      // data.time = 0
      return false;
    }

    Vector3f r = source.cross(edge1);
    float v = invAlpha * ray.direction().dot(r);
    if (v < 0.0f)
    {
      // data.u = 0
      // data.v = 0
      // data.time = 0
      return false;
    }

    // Hit data here
    // float t = f * edge2.dot(r);
    return true;
  }

  // TODO: Calculate triangle normal here
  Vector3f normal() const override
  {
    return normal_;
  }
private:
  Vector3u indices_;
  const Surface& owner_;
  Vector3f normal_;
};

}     // end of namespace raytracer

#endif // end of _RAY_TRIANGLE_
