#include "Material.h"

Material::Material()
{
}

Material::Material(Vector3f ambient,
                   Vector3f diffuse,
                   Vector3f specular) :
  amb(ambient),
  diff(diffuse),
  spec(specular),
  specPower(0.0f),
  reflectivity(0.0f)
{ }

Material::Material(Vector3f ambient,
                   Vector3f diffuse,
                   Vector3f specular,
                   float specPow) :
  amb(ambient),
  diff(diffuse),
  spec(specular),
  specPower(specPow),
  reflectivity(0.0f)
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
  return Material(Vector3f(0.2f, 0.0f, 0.0f), Vector3f(1.0f, 0.0f, 0.0f), Vector3f(0.0f, 0.0f, 0.0f));
}

Material Material::CreateGreenMat()
{
  return Material(Vector3f(0.0f, 0.2f, 0.0f), Vector3f(0.0f, 0.5f, 0.0f), Vector3f(0.5f, 0.5f, 0.5f));
}

Material Material::CreateBlueMat()
{
  return Material(Vector3f(0.0f, 0.0f, 0.2f), Vector3f(0.0f, 0.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f));
}

Material Material::CreateWhiteMat()
{
  return Material(Vector3f(0.2f, 0.2f, 0.2f), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f));
}
