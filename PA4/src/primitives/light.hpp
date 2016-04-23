/******************************************************************************
 *
 *  filename: Light.h
 *  author  : Do Won Cha
 *
 *****************************************************************************/

#pragma once

#ifndef _RAY_LIGHT_
#define _RAY_LIGHT_

#include <Eigen/Core>
#include "surface.hpp"

namespace raytracer
{

using namespace Eigen;

class Light : public Node
{
public:
  Vector3f ambient_, diffuse_;
  float intensity_;
public:
  Light(Vector3f position,
        Vector3f ambient,
        Vector3f diffuse,
        float intensity = 1.0f) :
    Node(position_),
    ambient_(ambient),
    diffuse_(diffuse),
    intensity_(intensity)
  {}
};

} // end of namespace raytracer

#endif //_RAY_LIGHT guard
