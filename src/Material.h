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
  Material(const glm::vec3& ambient, const glm::vec3& diff, const glm::vec3& spec, float specPower);
  Material(const glm::vec3& ambient, const glm::vec3& diff, const glm::vec3& spec, float specPower, float Reflection);

  // Factory functions for creating specific color materials
  static Material CreateRedMat();
  static Material CreateGreenMat();
  static Material CreateBlueMat();
  static Material CreateWhiteMat();

  glm::vec3 Ambient()       { return mAmbient;   }
  glm::vec3 Diffuse()       { return mDiffuse;   }
  glm::vec3 Specular()      { return mSpecular;  }
  float     SpecularPower() { return SpecPower; }
  float     Reflection()    { return ReflectionCoef; }
private:
  glm::vec3 mAmbient;
  glm::vec3 mDiffuse;
  glm::vec3 mSpecular;
  float SpecPower, ReflectionCoef;
};

#endif    // RAY_MATERIAL Header guard
