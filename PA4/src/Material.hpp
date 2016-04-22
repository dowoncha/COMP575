/******************************************************************************
 *
 *  filename: Material.h
 *  author  : Do Won Cha
 *
 *****************************************************************************/

#pragma once

#ifndef _RAY_MATERIAL_
#define _RAY_MATERIAL_

#include <Eigen/Core>

class Material
{
public:
  Material::Material()
  {
  }

  Material(Vector3f ambient,
           Vector3f diffuse,
           Vector3f specular) :
    amb(ambient),
    diff(diffuse),
    spec(specular),
    specPower(0.0f),
    reflectivity(0.0f)
  { }

  Material(Vector3f ambient,
           Vector3f diffuse,
           Vector3f specular,
           float specPow) :
    amb(ambient),
    diff(diffuse),
    spec(specular),
    specPower(specPow),
    reflectivity(0.0f)
  { }

  Material(Vector3f ambient,
           Vector3f diffuse,
           Vector3f specular,
           float specPower,
           float reflection) :
  amb(ambient),
  diff(diffuse),
  spec(specular),
  specPower(specPower),
  reflectivity(reflection)
  {
  }


  Vector3f ambient() const { return amb; }
  void ambient(Vector3f _amb)   { amb = _amb; }

  Vector3f diffuse() const { return diff; }
  void diffuse(Vector3f _diff) { diff = _diff };

  Vector3f specular() const { return spec; }
  void specular(Vector3f _spec) { spec = _spec};

  float specPower() const { return specPower; }
  void  specPower(Vector3f _specPower) { specPower = _specPower; }

  float reflection() const { return reflectivity; }
  void  reflection(float _reflect) { reflectivity = _reflect; }

  // Factory functions for creating specific color materials
  static Material CreateRedMat()
  {
    return Material(Vector3f(0.2f, 0.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f));
  }

  static Material CreateGreenMat()
  {
    return Material(Vector3f(0.0f, 0.2f, 0.0f), Vector3f(0.0f, 0.5f, 0.0f), Vector3f(0.5f, 0.5f, 0.5f));
  }

  static Material CreateBlueMat()
  {
    return Material(Vector3f(0.0f, 0.0f, 0.2f), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f));
  }

  static Material CreateWhiteMat()
  {
    return Material(Vector3f(0.2f, 0.2f, 0.2f), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f));
  }

private:
  Vector3f amb;
  Vector3f diff;
  Vector3f spec;
  float specPower, reflectivity;
};

#endif    // RAY_MATERIAL Header guard
