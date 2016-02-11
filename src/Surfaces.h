/******************************************************************************
 *
 *  filename: Surfaces.h
 *  author  : Do Won Cha
 *
 *****************************************************************************/

#pragma once

#ifndef _RAY_SURFACES_
#define _RAY_SURFACES_

#include "Primitives.h"
#include "Ray.h"
#include "Material.h"

class Node
{
public:
  Node() { }
  virtual ~Node() { }

  const Vector3f& GetPosition() const        { return Position; }
  void SetPosition(const Vector3f& p)        { Position = p; }
protected:
  Vector3f Position;
};

class Surface : public Node
{
public:
  Surface() : Node() { }

  virtual ~Surface()
  {
    delete Mat;
  }

  virtual bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const = 0;
  virtual bool Intersect(const Ray& ray, float tMax, float& t) const = 0;
  virtual bool Intersect(const Ray& ray, float tMax) const = 0;

  virtual Vector3f GetNormal(const Vector3f& p) const = 0;

  Material* GetMaterial() { return Mat; }
protected:
  Material* Mat;
};

class Sphere : public Surface
{
public:
  Sphere();
  Sphere(const Vector3f& center, float radius, Material* mat = nullptr);

  ~Sphere() {}

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
  Plane();
  Plane(const Vector3f& pos, const Vector3f& normal, Material* mat = nullptr);
  ~Plane();

  bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const override;
  bool Intersect(const Ray& ray, float tMax, float& t) const override;
  bool Intersect(const Ray& ray, float tMax) const override;
  
  Vector3f GetNormal() const { return Normal; }
  Vector3f GetNormal(const Vector3f& p) const override;
private:
  Vector3f Normal;
};

#endif // RAY_SURFACES end
