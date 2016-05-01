/******************************************************************************
 *
 *  filename: Surfaces.h
 *  author  : Do Won Cha
 *
 *****************************************************************************/

#pragma once
#ifndef _RAY_SURFACES_
#define _RAY_SURFACES_

#include <Eigen/Core>
#include <string>
#include <string>
#include "material.h"

class Ray;
class Surface;

namespace raytracer
{
using namespace Eigen;

/**
 *  Object's that have a position.
 */
class Node
{
public:
  Node() : position_(0.0f, 0.0f, 0.0f) { }
  Node(Vector3f position) : position_(position) { }

  virtual ~Node() { }

  Vector3f position() const         { return position_; }
  void position(Vector3f position) { position_ = position; }
protected:
  Vector3f position_;
};

/**
 *  Abstract class for objects that require a position and a material.
 */
class Surface : public Node
{
public:
  Surface(const Vector3f& position, std::string material = "") :
    Node(position),
    material_(material)
  {}

  virtual ~Surface() { }

  virtual bool intersect(const Ray& ray) const = 0;

  virtual bool intersect(const Ray& ray, HitData& hit) const = 0;

  void material(std::string material) { material_ = material; }

  std::string material() const { return material_.; }
protected:
  std::string material_;
};

using surface_t = std::shared_ptr<raytracer::Surface>;

/**
 *	Hit data is returned upon call to IntersectSurfaces.
 */
struct HitData
{
  HitData() :
    surface(nullptr),
    t(10000.0f)
  {}

  Vector3f point;
  Vector3f normal;
  Surface* surface;
  float t;
};

} // end of namespace raytracer

#endif // RAY_SURFACES end
