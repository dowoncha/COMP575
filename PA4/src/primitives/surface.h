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
#include "material.h"
#include "ray.h"

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
  void set_position(Vector3f position) { position_ = position; }
protected:
  Vector3f position_;
};

/**
 *  Abstract class for objects that require a position and a material.
 */
class Surface : public Node
{
public:
  Surface(Vector3f position) :
    Node(position)
  {}

  Surface(material_t material) :
    Node()
  {}

  Surface(Vector3f position, material_t material) :
    Node(position),
    material_(material)
  { }

  virtual ~Surface() { }

  virtual bool Intersect(const Ray& ray) const = 0;

  virtual bool Intersect(const Ray& ray, Vector3f& hit_point, Vector3f& hit_normal) const = 0;

  void set_material(material_t material) { material_ = material; }
  Material* material() const { return material_.get(); }
protected:
  material_t material_;
};

using surface_t = std::shared_ptr<raytracer::Surface>;

/**
 *	Hit data is returned upon call to IntersectSurfaces.
 */
struct HitData
{
  Vector3f point;
  Vector3f normal;
  Surface* surface;

  HitData() :
    surface(nullptr)
  { }
};

} // end of namespace raytracer

#endif // RAY_SURFACES end
