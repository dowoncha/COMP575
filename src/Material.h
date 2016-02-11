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

  // Factory functions for creating specific color materials
  static Material* CreateRedMat()
  {
    return new Material(Vector3f(0.2f, 0.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), 0);
  }

  static Material* CreateGreenMat()
  {
    return new Material(Vector3f(0.0f, 0.2f, 0.0f), Vector3f(0.0f, 0.5f, 0.0f), Vector3f(0.5f, 0.5f, 0.5f), 32.0f);
  }

  static Material* CreateBlueMat()
  {
    return new Material(Vector3f(0.0f, 0.0f, 0.2f), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f), 0.0f);
  }

  static Material* CreateWhiteMat()
  {
    return new Material(Vector3f(0.2f, 0.2f, 0.2f), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f), 8.0f);
  }

  Vector3f GetAmbient()     { return Ambient;   }
  Vector3f GetDiffuse()     { return Diffuse;   }
  Vector3f GetSpecular()    { return Specular;  }
  float    GetSpecularPow() { return SpecPower; }
private:
  Vector3f Ambient;
  Vector3f Diffuse;
  Vector3f Specular;
  float SpecPower;
};

#endif    // RAY_MATERIAL Header guard
