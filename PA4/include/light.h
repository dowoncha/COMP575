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
#include "surface.h"

namespace raytracer
{

using namespace Eigen;

class Light : public Node
{
public:
  Light(Vector3f position,
        Vector3f ambient,
        Vector3f diffuse,
        float intensity = 1.0f) :
    Node(position),
    ambient_(ambient),
    diffuse_(diffuse),
    intensity_(intensity)
  {}

  Vector3f ambient() const { return ambient_; }
  Vector3f diffuse() const { return diffuse_; }
  float intensity() const { return intensity_; }
private:
  Vector3f ambient_, diffuse_;
  float intensity_;
};

using light_t         = std::shared_ptr<raytracer::Light>;

} // end of namespace raytracer

#endif //_RAY_LIGHT guard
