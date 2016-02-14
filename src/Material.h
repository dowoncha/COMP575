/******************************************************************************
 *
 *  filename: Material.h
 *  author  : Do Won Cha
 *
 *****************************************************************************/

#include "Primitives.h"

#pragma once

#ifndef _RAY_MATERIAL_
#define _RAY_MATERIAL_

class Material
{
public:
  Material();
  Material(const Vector3f& ambient, const Vector3f& diff, const Vector3f& spec, float specPower);
  Material(const Vector3f& ambient, const Vector3f& diff, const Vector3f& spec, float specPower, float Reflection);

  // Factory functions for creating specific color materials
  static Material* CreateRedMat();
  static Material* CreateGreenMat();
  static Material* CreateBlueMat();
  static Material* CreateWhiteMat();

  Vector3f GetAmbient()     { return Ambient;   }
  Vector3f GetDiffuse()     { return Diffuse;   }
  Vector3f GetSpecular()    { return Specular;  }
  float    GetSpecularPow() { return SpecPower; }
  float    GetReflection()  { return Reflection; }
private:
  Vector3f Ambient;
  Vector3f Diffuse;
  Vector3f Specular;
  float SpecPower, Reflection;
};

#endif    // RAY_MATERIAL Header guard
