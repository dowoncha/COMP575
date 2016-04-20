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

#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Image.h"
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
    /**
     *  Resize the window
     *  @param w width
     *  @param h height
     */
    void Resize(int w, int h);

    /**
     *  [Render description]
     *  @param image [description]
     */
    void Trace();

    void Render() const;

    Vector3f Trace(const Ray& ray, int depth) const;
private:
    Vector3f Shade(const Ray& ray, const HitData& Data) const;

    // Check whether shadow ray is blocked by any surface
    bool ShadowTrace(const HitData& Data) const;

    Vector3f CalculateDiffuse(const HitData& data, Light* light) const;

    Vector3f CalculateSpecular(const Ray& ray, const HitData& data, Light* light) const;

    // These are for rendering the black and white trace images.
    // Might be useful for debugging, mostly for assignment though.
    void BWRender(Image& image) const;
    Vector3f BWTrace(const Ray& ray) const;

    void SetSampleRate(int s);

    Vector3f Sampler(int x, int y) const;

    // Uses evenly spaced ray's in the pixel. Uses sample * sample.
    Vector3f UniformSampler(int x, int y) const;

    // Uses a random number generator in the [0,1] space to perturb original ray
    // sample x sample rays.
    Vector3f RandomSampler(int x, int y) const;
private:
    // Main scene to draw
    const Scene& mScene;

    std::vector<Vector4f> frameBuffer;
    Camera MainCamera;

    // These should be camera variables. Will use eventually
    float fov, angle;
    float AspectRatio;

    int SampleRate;
    int MaxTraceDepth;
};

#endif /* end of include guard: _RAY_TRACER_ */
