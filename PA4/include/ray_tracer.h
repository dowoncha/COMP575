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

enum PostProcess
{
};

enum PostProcessRate
{
};

class RayTracer
{
  typedef std::vector<Vector3f> buffer_t
  typedef size_t frame_buffer_size_t;
public:
  /**
   *  Set the width and height of the camera.
   */
  RayTracer(int width, int height);

  ~RayTracer();

  void bwrender(Scene* scene);

  /**
   *  Ray trace the input scene and fill the frame buffer with traced result
   *  @param scene raw pointer to render
   */
  void render(Scene* scene);

  void setMaxTraceDepth(int newDepth);

  // Enable/disable shadow calculations
  void enableShadows();
  void disableShadows();

  // Returns a raw pointer to the frame buffer data for gl access.
  Scene* frameBuffer();
private:
  /**
   *  Trace a ray through the scene recursively.
   *  @param ray Ray to shoot through scene.
   *  @param depth should alwyas be 0, else used recursively.
   *  @return HitData information on the ray trace
   */
  Vector4f trace(Scene* scene, const Ray& ray, int depth = 0) const;

  // Use ray and hit data to calculate the color at the point.
  Vector4f localShading(Scene* scene, const Ray& ray, const HitData& Data) const;

  /**
   *  Gamma encode everything.
   *  @param  color in
   *  @return       gamma encoded color
   */
  static Vector3f gammaEncode(const Vector3f& color);
private:
  // frame buffer
  buffer_t frame_buffer_;

  // Main scene to draw
  Camera camera_;

  frame_buffer_size_t size_;

  float max_trace_time_;      // Trace maximum time
  int max_trace_depth_;

  bool bShadows_;
};

} // end of namespace raytracer

#endif /* end of include guard: _RAY_TRACER_ */
