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

class Light : public Node
{
public:
  glm::vec3 Ambient, Diffuse;
  float Intensity;

public:
  Light();
  Light(const glm::vec3& pos,
        const glm::vec3& ambient,
        const glm::vec3 & diff);

  ~Light();

  void SetIntensity(float i);
};

#endif //_RAY_LIGHT guard
