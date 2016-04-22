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
#include "Ray.hpp"
#include "Material.hpp"

using namespace Eigen;

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
    tMax(100000.0f),
    t(0),
    HitSurface(nullptr)
  { }
};

/**
 *  Object's that have a position.
 */
class Node
{
public:
  Node() { }

  Node(Vector3f position) : position_(position) { }

  virtual ~Node() { }

  Vector3f position() const         { return position_; }
  void set_position(Vector3f p) { position_ = position; }
protected:
  Vector3f position_;
};

/**
 *  Abstract class for objects that require a position and a material.
 */
class Surface : public Node
{
public:
  Surface() :
    Node(),
  { }

  void surface()
  {

  }

  virtual ~Surface() { }

  virtual bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const = 0;
  virtual bool Intersect(const Ray& ray, float tMax, float& t) const = 0;
  virtual bool Intersect(const Ray& ray, float tMax) const = 0;

  virtual Vector3f GetNormal(const Vector3f& point) const = 0;

  void SetMaterial(Material* newMat)
  {
    material_ = newMat;
  }

  Material material() const { return *matRef; }
protected:
  Material* material_;
};

#endif // RAY_SURFACES end
