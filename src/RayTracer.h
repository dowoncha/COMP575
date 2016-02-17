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

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <vector>
#include <random>

#include "Scene.h"
#include "Primitives.h"
#include "Camera.h"
#include "Light.h"
#include "Image.h"
#include "easylogging++.h"


class RayTracer
{
public:
    RayTracer(const Scene& scene, int sWidth, int sHeight);

    ~RayTracer();

    // Call this and pass an image object to it, to write result to the image buffer
    void Render(Image& image) const;

    // Pass a ray to trace through the scene and get a resulting color
    Vector3f Trace(const Ray& ray, int depth) const;

    // Determine the diffuse and specular color
    Vector3f Shade(const Ray& ray, const HitData& Data) const;

    // Pretty obvious
    Vector3f CalculateDiffuse(const HitData& data, Light* light) const;
    Vector3f CalculateSpecular(const Ray& ray, const HitData& data, Light* light) const;

    // DEPRECATED:
    // Basically does diffuse and specular in one function
    Vector3f CalculateLight(const Ray& ray, const HitData& Data, Light* light) const;

    // Types for which sampling to use
    enum PostProcess
    {
        UniformSampling,
        RandomSampling
    } SamplingType;

    void SetSampleRate(int s);

    // Switch function for type of sampler to use depending on the SamplingType
    Vector3f Sampler(int x, int y) const;

    // Uses evenly spaced ray's in the pixel. Uses sample * sample.
    Vector3f UniformSampler(int x, int y) const;

    // Uses a random number generator in the [0,1] space to perturb original ray
    // sample x sample rays.
    Vector3f RandomSampler(int x, int y) const;
private:
    // Main scene to draw
    const Scene& mScene;

    Camera MainCamera;

    int ScreenWidth;
    int ScreenHeight;

    // These should be camera variables. Will use eventually
    float fov, angle;
    float AspectRatio;

    int SampleRate;
    int MaxTraceDepth;
};

#endif /* end of include guard: _RAY_TRACER_ */
