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

namespace raytracer
{

using namespace Eigen;

class Surface;
class Ray;

/**
 *	Hit data is returned upon call to IntersectSurfaces.
 */
struct HitData
{
  Vector3f hit_point, hit_time;
  Vector3f normal;
  float t, tMax;
  Surface* hit_surface;

  HitData() :
    t(0),
    tMax(10000.0f),
    hit_surface(nullptr)
  { }
};

/**
 *  Object's that have a position.
 */
class Node
{
public:

  Node() : position_(0.0f) { }
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
  Surface(Vector3f position, std::string material_name = "") :
    Node(position),
    material_name_(material_name)
  { }

  virtual ~Surface() { }

  virtual bool Intersect(const Ray& ray, HitData& hit) = 0;

  virtual Vector3f normal() const;

  void set_material(std::string material_name) { material_name_ = material_name; }
  std::string material() const { return material_name_; }
protected:
  std::string material_name_;
};

} // end of namespace raytracer

#endif // RAY_SURFACES end
