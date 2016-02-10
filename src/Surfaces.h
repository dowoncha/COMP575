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
  Node() { std::cout<<"Node constructor called\n"; }
  //Node(const Vector3f& pos) : Position(pos) { }
  virtual ~Node() { }

  const Vector3f& GetPosition() const        { return Position; }
  void SetPosition(const Vector3f& p)        { Position = p; }
protected:
  Vector3f Position;
};

class Surface : public Node
{
public:
  Surface() : Node() { std::cout << "Surface constructor called\n"; }
  //Surface(const Vector3f& pos ) : Node(pos) { }
  virtual ~Surface()
  {
    delete Mat; 
  }

  virtual bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const = 0;
  virtual Vector3f GetNormal(const Vector3f& Point) const = 0;
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
  Vector3f GetNormal(const Vector3f& Point) const override;
private:
  float Radius, Radius2;
};

class Plane : public Surface
{
public:
  Plane();
  Plane(const Vector3f& point, const Vector3f& normal, Material* mat = nullptr);
  ~Plane();

  bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const override;
  Vector3f GetNormal(const Vector3f& Point) const override;
private:
  Vector3f Normal;
};

#endif // RAY_SURFACES end
