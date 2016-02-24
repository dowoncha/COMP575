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
  virtual ~Node() { }

  glm::vec3 GetPosition() const         { return Position; }
  void SetPosition(const glm::vec3& p) { Position = p; }
protected:
  glm::vec3 Position;
};

class Surface : public Node
{
public:
  Surface(const Material & mat);

  virtual ~Surface() { }

  virtual bool Intersect(const Ray& ray, float tMax, float& t, glm::vec3& Point) const = 0;
  virtual bool Intersect(const Ray& ray, float tMax, float& t) const = 0;
  virtual bool Intersect(const Ray& ray, float tMax) const = 0;

  virtual glm::vec3 GetNormal(const glm::vec3& p) const = 0;

  Material Mat() const { return mMaterial; }
protected:
  const Material& mMaterial;
};

class Sphere : public Surface
{
public:
  //Sphere();
  Sphere(const glm::vec3& center, float radius, const Material& mat);

  ~Sphere();

  bool Intersect(const Ray& ray, float tMax, float& t, glm::vec3& Point) const override;
  bool Intersect(const Ray& ray, float tMax, float& t) const override;
  bool Intersect(const Ray& ray, float tMax) const override;
  glm::vec3 GetNormal(const glm::vec3& p) const override;
private:
  float Radius, Radius2;
};

class Plane : public Surface
{
public:
  //Plane();
  Plane(const glm::vec3& pos, const glm::vec3& normal, Material const & mat);
  ~Plane();

  bool Intersect(const Ray& ray, float tMax, float& t, glm::vec3& Point) const override;
  bool Intersect(const Ray& ray, float tMax, float& t) const override;
  bool Intersect(const Ray& ray, float tMax) const override;

  glm::vec3 GetNormal() const;
  glm::vec3 GetNormal(const glm::vec3& p) const override;
private:
  glm::vec3 Normal;
};

#endif // RAY_SURFACES end
