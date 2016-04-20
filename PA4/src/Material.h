/******************************************************************************
 *
 *  filename: Material.h
 *  author  : Do Won Cha
 *
 *****************************************************************************/

#pragma once

#ifndef _RAY_MATERIAL_
#define _RAY_MATERIAL_

#include <glm/vec3.hpp>

class Material
{
public:
  Material();
  Material(const Vector3f& ambient, const Vector3f& diffuse, const Vector3f& specular, float specPower);
  Material(const Vector3f& ambient, const Vector3f& diffuse, const Vector3f& specular, float specPower, float Reflection);

  // Factory functions for creating specific color materials
  static Material CreateRedMat();
  static Material CreateGreenMat();
  static Material CreateBlueMat();
  static Material CreateWhiteMat();

  Vector3f ambient()           { return amb;   }
  void ambient(Vector3f _amb)   { amb = _amb; }

  Vector3f diffuse()       { return diff;   }
  void diffuse(Vector3f _diff) { diff = _diff };

  Vector3f specular()      { return spec;  }
  void specular(Vector3f _spec) { spec = _spec};

  float specularPower() { return specPower; }
  void  specPower(Vector3f _specPower) { specPower = _specPower; }

  float reflection() { return reflectivity; }
  void  reflection(float _reflect) { reflectivity = _reflect; }

private:
  Vector3f amb;
  Vector3f diff;
  Vector3f spec;
  float specPower, reflectivity;
};

#endif    // RAY_MATERIAL Header guard
