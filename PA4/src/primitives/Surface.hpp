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

/**
 *	Hit data is returned upon call to IntersectSurfaces.
 */
struct HitData
{
public:
  Vector3f hitPoint, hitTime;
  Vector3f normal;
  float t, tMax;
  Surface* HitSurface;
public:
  HitData() :
    tMax(100000.0f),
    t(0),
    HitSurface(nullptr)
  { }
};

class Node
{
public:
  Node() { }

  Node(Vector3f position) : pos(position) { }

  virtual ~Node() { }

  Vector3f position() const         { return pos; }
  void position(Vector3f p) { pos = p; }
protected:
  Vector3f pos;
};

class Surface : public Node
{
public:
  Surface(const Material& mat) :
    Node(), mMaterial(mat)
  { }

  virtual ~Surface() { }

  virtual bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const = 0;
  virtual bool Intersect(const Ray& ray, float tMax, float& t) const = 0;
  virtual bool Intersect(const Ray& ray, float tMax) const = 0;

  virtual Vector3f GetNormal(const Vector3f& point) const = 0;

  Material material() const { return mMaterial; }
protected:
  const Material& matRef;
};

#endif // RAY_SURFACES end
