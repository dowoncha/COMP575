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

#include <GL/glew.h>
#include <GL/glut.h>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <vector>
#include <random>
#include <memory>

#include <Eigen/Core>

#include "easylogging++.h"
#include "scene.hpp"
#include "primitives/material.hpp"
#include "primitives/camera.hpp"
#include "utility.h"

namespace raytracer
{

using namespace Eigen;

// Types for which sampling to use
enum PostProcess
{
  NoSampling,
  UniformSampling,
  RandomSampling
} SamplingType;

class RayTracer
{
  // This is to define a member function pointer type
  typedef Vector4f (RayTracer::*SamplingFunction)(int x, int y);
  typedef size_t frame_buffer_size_t;
public:
  /**
   *  Takes a scene and the screen width and height
   */
  RayTracer(int* argc, char** argv);

  ~RayTracer();

  void initialize(std::unique_ptr<Scene>& scene);
  /**
   *  Resize the window
   *  @param w width
   *  @param h height
   */
  void resize(int w, int h);

  // Run glut main loop and draw();
  void run();

  void set_sampling_type(PostProcess type);

  // Set anti-aliasing sample rate, 1x, 2x, 4x, 8x, 16x
  void set_sample_rate(int sample_rate);
private:
  void Idle();
  /**
   *  gl display function
   */
  void Display();

  /**
   *  Ray trace render function called by idle function
   */
  void Render();
  /**
   *  Trace a ray through the scene recursively.
   *  @param ray Ray to shoot through scene.
   *  @param depth should alwyas be 0, else used recursively.
   *  @return HitData information on the ray trace
   */
  Vector4f Trace(const Ray& ray, int depth) const;

  // Use ray and hit data to calculate the color at the point.
  Vector4f LocalShading(const Ray& ray, const HitData& Data) const;

  // Simply shoots a ray.
  Vector4f NoSampling(int x, int y);

  // Uses evenly spaced ray's in the pixel. Uses sample * sample.
  Vector4f UniformSampling(int x, int y);

  // Uses a random number generator in the [0,1] space to perturb original ray
  // sample * sample rays.
  Vector4f RandomSampling(int x, int y);
private:
  // Main scene to draw
  std::unique_ptr<Scene> scene_;
  std::unique_ptr<Camera> camera_;

  // frame buffer
  std::vector<Vector4f> frame_buffer_;
  frame_buffer_size_t size_;

  // Anti-aliasing settings
  int sample_rate_;
  SamplingFunction sampler;   // Sampling function pointer used to call samplying type

  float max_trace_time_;      // Trace maximum time
  int max_trace_depth_;       // Trace recursion maximum depth
};

} // end of namespace raytracer

#endif /* end of include guard: _RAY_TRACER_ */
