#include "Material.h"

Material::Material()
{
}

Material::Material(const Vector3f& ambient, const Vector3f& diff, const Vector3f& spec, float specPower) :
  Ambient(ambient),
  Diffuse(diff),
  Specular(spec),
  SpecPower(specPower)
{
}
