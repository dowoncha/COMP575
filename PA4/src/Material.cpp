#include "Material.h"

Material::Material()
{
}

Material::Material(glm::vec3 const & ambient,
                   glm::vec3 const & diff,
                   glm::vec3 const & spec,
                   float specPower) :
  mAmbient(ambient),
  mDiffuse(diff),
  mSpecular(spec),
  SpecPower(specPower),
  ReflectionCoef(0.0f)
{
}

Material::Material(glm::vec3 const & ambient,
                   glm::vec3 const & diff,
                   glm::vec3 const & spec,
                   float specPower,
                   float reflection) :
mAmbient(ambient),
mDiffuse(diff),
mSpecular(spec),
SpecPower(specPower),
ReflectionCoef(reflection)
{
}

// Factory functions for creating specific color materials
Material Material::CreateRedMat()
{
  return Material(glm::vec3(0.2f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0);
}

Material Material::CreateGreenMat()
{
  return Material(glm::vec3(0.0f, 0.2f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, 0.8f);
}

Material Material::CreateBlueMat()
{
  return Material(glm::vec3(0.0f, 0.0f, 0.2f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f);
}

Material Material::CreateWhiteMat()
{
  return Material(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 8.0f, 0.8f);
}
