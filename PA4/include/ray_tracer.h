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

namespace raytracer
{

class Scene;
class Camera;
class Ray;
struct HitData;

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

  void setMaxTraceDepth(int newDepth);

  void enableShadows();

  void disableShadows();
private:
  /**
   *  Trace a ray through the scene recursively.
   *  @param ray Ray to shoot through scene.
   *  @param depth should alwyas be 0, else used recursively.
   *  @return HitData information on the ray trace
   */
  Vector4f trace(Scene* scene, const Ray& ray, int depth);

  // Use ray and hit data to calculate the color at the point.
  Vector4f localShading(Scene* scene, const Ray& ray, const HitData& Data) const;

  static Vector4f gammaEncode(const Vector4f& color);

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