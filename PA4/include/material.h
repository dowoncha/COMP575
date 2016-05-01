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

namespace raytracer
{

using namespace Eigen;

class Material
{
public:
  Material(Vector3f ambient,
           Vector3f diffuse,
           Vector3f specular,
           float specular_power = 0.0f,
           float reflection = 0.0f) :
    ambient_(ambient),
    diffuse_(diffuse),
    specular_(specular),
    specular_power_(specular_power),
    reflectivity_(reflection)
  { }

  // Ambient getter and setter
  Vector3f ambient() const { return ambient_; }
  void ambient(const Vector4f& ambient)   { ambient_ = ambient; }

  // Diffuse getter and setter
  Vector3f diffuse() const { return diffuse_; }
  void diffuse(const Vector4f& diffuse) { diffuse_ = diffuse; }

  // Specular getter and setter
  Vector3f specular() const { return specular_; }
  void specular(const Vector3f& specular) { specular_ = specular; }

  // Specular power getter and setters
  float specularPower() const { return specular_power_; }
  void  specularPower(float specular_power) { specular_power_ = specular_power; }

  // Reflection getter and setters
  float reflectivity() const { return reflectivity_; }
  void  reflectivity(float reflectivity) { reflectivity_ = reflectivity; }

  std::string name() { return name_; }
  void set_name(std::string name) { name_ = name; }
private:
  Vector3f ambient_;
  Vector3f diffuse_;
  Vector3f specular_;
  float specular_power_, reflectivity_;
};

using material_t      = std::shared_ptr<raytracer::Material>;

} // end of namespace raytracer

#endif    // RAY_MATERIAL Header guard
