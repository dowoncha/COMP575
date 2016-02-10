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
private:
  Vector3f Position;
public:
  Node() : Position(Vector3f(0)) { }
  virtual ~Node() { }

  const Vector3f& GetPosition() const { return Position; }
  void SetPosition(Vector3f p)        { Position = p; }
};

class Surface : public Node
{
public:
  Surface() : Node() { }
  virtual ~Surface() { }

  virtual bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const = 0;
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
private:
  Vector3f Center;
  float Radius, Radius2;
  Material* Mat;
};

class Plane : public Surface
{
public:
  Plane(Vector3f point, Vector3f normal);
  ~Plane();

  bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const override;
private:
  Vector3f Point;
  Vector3f Normal;
};

#endif // RAY_SURFACES end
