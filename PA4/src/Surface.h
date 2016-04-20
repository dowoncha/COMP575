/******************************************************************************
 *
 *  filename: Surfaces.h
 *  author  : Do Won Cha
 *
 *****************************************************************************/

#pragma once
#ifndef _RAY_SURFACES_
#define _RAY_SURFACES_

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "Ray.h"
#include "Material.h"

class Node
{
public:
  Node() { }

  Node(Vector3f position) : pos(position) { }

  virtual ~Node() { }

  Vector3f position() const         { return pos; }
  void position(const Vector3f& p) { pos = p; }
protected:
  Vector3f pos;
};

class Surface : public Node
{
public:
  Surface(const Material & mat);

  virtual ~Surface() { }

  virtual bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const = 0;
  virtual bool Intersect(const Ray& ray, float tMax, float& t) const = 0;
  virtual bool Intersect(const Ray& ray, float tMax) const = 0;

  virtual Vector3f GetNormal(const Vector3f& point) const = 0;

  Material Mat() const { return mMaterial; }
protected:
  const Material& mMaterial;
};

class Sphere : public Surface
{
public:
  //Sphere();
  Sphere(const Vector3f& center, float radius, const Material& mat);

  ~Sphere();

  bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const override;
  bool Intersect(const Ray& ray, float tMax, float& t) const override;
  bool Intersect(const Ray& ray, float tMax) const override;
  Vector3f GetNormal(const Vector3f& p) const override;
private:
  float Radius, Radius2;
};

class Plane : public Surface
{
public:
  //Plane();
  Plane(const Vector3f& pos, const Vector3f& normal, Material const & mat);
  ~Plane();

  bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const override;
  bool Intersect(const Ray& ray, float tMax, float& t) const override;
  bool Intersect(const Ray& ray, float tMax) const override;

  Vector3f GetNormal() const;
  Vector3f GetNormal(const Vector3f& p) const override;
private:
  Vector3f Normal;
};

class Triangle : public Surface
{

}

#endif // RAY_SURFACES end
