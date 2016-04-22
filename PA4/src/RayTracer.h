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

#include <Eigen/Core>

#include "Scene.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "easylogging++.h"

// Types for which sampling to use
enum PostProcess
{
    UniformSampling,
    RandomSampling
} SamplingType;

class RayTracer
{
public:
  /**
   *  Takes a scene and the screen width and height
   */
  RayTracer(int& argc, char** argv);

  ~RayTracer();

  void init();
  /**
   *  Resize the window
   *  @param w width
   *  @param h height
   */
  void resize(int w, int h);

  void run();

  // Set anti-aliasing sample rate, 1x, 2x, 4x, 8x, 16x
  void sampleRate(int s);
private:
  /**
   *  [Render description]
   *  @param image [description]
   */
  void render();

  /**
   *  Trace a ray through the scene recursively.
   *  @param ray Ray to shoot through scene.
   *  @param depth should alwyas be 0, else used recursively.
   *  @return HitData information on the ray trace
   */
  Vector3f trace(const Ray& ray, int depth) const;

  // Use ray and hit data to calculate the color at the point.
  Vector3f shade(const Ray& ray, const HitData& Data) const;

  Vector3f sampler(int x, int y) const;

  // Uses evenly spaced ray's in the pixel. Uses sample * sample.
  Vector3f UniformSampler(int x, int y) const;

  // Uses a random number generator in the [0,1] space to perturb original ray
  // sample x sample rays.
  Vector3f RandomSampler(int x, int y) const;

  // Main scene to draw
  const Scene& mScene;

  size_t frameBufferSize;

  std::vector<Vector4f> frameBuffer;
  Camera MainCamera;

  // These should be camera variables. Will use eventually
  float fov, angle;
  float AspectRatio;

  int SampleRate;
  int MaxTraceDepth;
};

#endif /* end of include guard: _RAY_TRACER_ */
