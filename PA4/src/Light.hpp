/******************************************************************************
 *
 *  filename: Light.h
 *  author  : Do Won Cha
 *
 *****************************************************************************/

#pragma once

#ifndef _RAY_LIGHT_
#define _RAY_LIGHT_

#include "Surface.h"

#include "Light.h"

Light::Light() :
  Node()
{
}

Light::Light(Vector3f const & pos,
             Vector3f const & ambient,
             Vector3f const & diffuse) :
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


class Light : public Node
{
public:
  Vecotr3f pos;
  Vector3f amb, diff;
  float intensity;

public:
  Light() : Node() { }

  Light(const Vector3f& position,
        const Vector3f& ambient,
        const Vector3f& diffuse) :
  Node(pos),
  amb(ambient),
  diff(diffuse),
  intensity(1.0f)
  {
  }

  ~Light();

  void SetIntensity(float i);
};

#endif //_RAY_LIGHT guard
