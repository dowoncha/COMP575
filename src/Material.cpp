#include "Material.h"

Material::Material()
{
}

Material::Material(const Vector3f& ambient, const Vector3f& diff, const Vector3f& spec, float specPower) :
  Ambient(ambient),
  Diffuse(diff),
  Specular(spec),
  SpecPower(specPower),
  Reflection(0.01f)
{
}

Material::Material(const Vector3f& ambient, const Vector3f& diff, const Vector3f& spec, float specPower, float reflection) :
Ambient(ambient),
Diffuse(diff),
Specular(spec),
SpecPower(specPower),
Reflection(reflection)
{
}

// Factory functions for creating specific color materials
Material Material::CreateRedMat()
{
  return Material(Vector3f(0.2f, 0.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f), 0);
}

Material Material::CreateGreenMat()
{
  return Material(Vector3f(0.0f, 0.2f, 0.0f), Vector3f(0.0f, 0.5f, 0.0f), Vector3f(0.5f, 0.5f, 0.5f), 32.0f, 0.8f);
}

Material Material::CreateBlueMat()
{
  return Material(Vector3f(0.0f, 0.0f, 0.2f), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f), 0.0f);
}

Material Material::CreateWhiteMat()
{
  return Material(Vector3f(0.2f, 0.2f, 0.2f), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f), 8.0f, 0.8f);
}
