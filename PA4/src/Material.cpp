#include "Material.h"

Material::Material()
{
}

Material::Material(Vector3f ambient,
                   Vector3f diffuse,
                   Vector3f specular,
                   float specPower) :
  amb(ambient),
  diff(diffuse),
  spec(specular),
  SpecPower(specPower),
  ReflectionCoef(0.0f)
{
}

Material::Material(Vector3f ambient,
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
