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

  static Material* CreateRedMaterial()
  {
    return new Material(Vector3f(0.2f, 0.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), 0);
  }

  static Material* CreateGreenMaterial()
  {
    return new Material(Vector3f(0.0f, 0.5f, 0.0f), Vector3f(0.0f, 0.5f, 0.0f), Vector3f(0.5f, 0.5f, 0.5f), 32.0f);
  }

private:
  Vector3f Ambient;
  Vector3f Diffuse;
  Vector3f Specular;
  float SpecPower;
};

#endif    // RAY_MATERIAL Header guard
