#include "Light.h"

Light::Light() :
  Node()
{
}

Light::Light(glm::vec3 const & pos,
             glm::vec3 const & ambient,
             glm::vec3 const & diffuse) :
  Node(),
  Ambient(ambient),
  Diffuse(diffuse),
  Intensity(1.0f)
{
  Position = pos;
}

Light::~Light()
{
}

void Light::SetIntensity(float i)
{
  Intensity = i;
}
