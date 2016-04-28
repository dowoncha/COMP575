/**
 *
 *  filename    :   RayTracer.cpp
 *  author      :   Do Won Cha
 *  content     :   Main ray tracer class give it a scene to get an image.
 *
 */

#pragma once
#ifndef _RAY_TRACER_
#define _RAY_TRACER_

#define NOMINMAX

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <vector>
#include <random>
#include <memory>

#include <Eigen/Core>

#include "scene.h"
#include "primitives/material.h"
#include "primitives/camera.h"
#include "utility.h"

namespace raytracer
{
using namespace Eigen;

class RayTracer
{
  typedef size_t frame_buffer_size_t;
public:
  /**
   *  Takes a scene and the screen width and height
   */
  RayTracer();

  RayTracer(int width, int height);

  ~RayTracer();

  void bwrender(Scene* scene);

  /**
   *  Ray trace render function called by idle function
   */
  void render(Scene* scene);

  void set_max_trace_depth(int new_depth);

  void toggle_shadows();
private:
  /**
   *  Trace a ray through the scene recursively.
   *  @param ray Ray to shoot through scene.
   *  @param depth should alwyas be 0, else used recursively.
   *  @return HitData information on the ray trace
   */
  Vector4f trace(Scene* scene, const Ray& ray, int depth);

  // Use ray and hit data to calculate the color at the point.
  Vector4f local_shading(Scene* scene, const Ray& ray, const HitData& Data) const;

  Vector4f GammaEncode(const Vector4f& color)
  {
    float gamma = 1.0f / 2.2f;

    Vector4f out;

    out(0) = std::pow(color(0), gamma);
    out(1) = std::pow(color(1), gamma);
    out(2) = std::pow(color(2), gamma);

    out(0) = Utility::PinToUnit(out(0));
    out(1) = Utility::PinToUnit(out(1));
    out(2) = Utility::PinToUnit(out(2));

    out(3) = 1.0f;

    return out;
  }
public:
  // frame buffer
  std::vector<Vector4f> frame_buffer_;

  // Main scene to draw
  std::unique_ptr<Camera> camera_;
private:
  frame_buffer_size_t size_;

  float max_trace_time_;      // Trace maximum time
  int max_trace_depth_;

  bool bShadows_;
};

} // end of namespace raytracer

#endif /* end of include guard: _RAY_TRACER_ */
