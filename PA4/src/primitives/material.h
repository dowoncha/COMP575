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
#include <string>
#include <memory>

using namespace Eigen;

namespace raytracer
{

class Material
{
public:
  Material(Vector4f ambient,
           Vector4f diffuse,
           Vector4f specular,
           float specular_power = 0.0f,
           float reflection = 0.0f) :
    ambient_(ambient),
    diffuse_(diffuse),
    specular_(specular),
    specular_power_(specular_power),
    reflectivity_(reflection)
  { }

  // Ambient getter and setter
  Vector4f ambient() const { return ambient_; }
  void set_ambient(Vector4f ambient)   { ambient_ = ambient; }

  // Diffuse getter and setter
  Vector4f diffuse() const { return diffuse_; }
  void set_diffuse(Vector4f diffuse) { diffuse_ = diffuse; }

  // Specular getter and setter
  Vector4f specular() const { return specular_; }
  void set_specular(Vector4f specular) { specular_ = specular; }

  // Specular power getter and setters
  float specular_power() const { return specular_power_; }
  void  set_specular_power(float specular_power) { specular_power_ = specular_power; }

  // Reflection getter and setters
  float reflectivity() const { return reflectivity_; }
  void  set_reflectivity(float reflectivity) { reflectivity_ = reflectivity; }

  std::string name() { return name_; }
  void set_name(std::string name) { name_ = name; }
private:
  std::string name_;
  Vector4f ambient_;
  Vector4f diffuse_;
  Vector4f specular_;
  float specular_power_, reflectivity_;
};

using material_t      = std::shared_ptr<raytracer::Material>;

} // end of namespace raytracer

#endif    // RAY_MATERIAL Header guard
