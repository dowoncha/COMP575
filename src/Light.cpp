#include "Light.h"

Light::Light() :
  Node()
{
}

Light::~Light()
{
}

Light::Light(const Vector3f& pos, const Vector3f& ambient, const Vector3f& diffuse) :
  Node(),
  Ambient(ambient),
  Diffuse(diffuse),
  Intensity(1.0f)
{
  Position = pos;
}

void Light::SetIntensity(float i)
{
  Intensity = i;
}
