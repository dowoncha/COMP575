#include "Light.h"

Light::Light() :
  Node()
{
}

Light::~Light()
{

}

Light::Light(const Vector3f& pos) :
  Node(),
  Ambient(1.0f),
  Diffuse(1.0f)
{
  Position = pos;
}
