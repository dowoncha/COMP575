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

class RayTracer
{
public:
    RayTracer(const Scene& scene, int sWidth, int sHeight);

    ~RayTracer();

    void Resize(int w, int h);

    void Render(Image& image) const;

    std::vector<Pixel> Render() const;

    glm::vec3 Trace(const Ray& ray, int depth) const;

    glm::vec3 Shade(const Ray& ray, const HitData& Data) const;

    // Check whether shadow ray is blocked by any surface
    bool ShadowTrace(const HitData& Data) const;

    glm::vec3 CalculateDiffuse(const HitData& data, Light* light) const;

    glm::vec3 CalculateSpecular(const Ray& ray, const HitData& data, Light* light) const;

    enum PostProcess
    {
        UniformSampling,
        RandomSampling
    } SamplingType;

    void SetSampleRate(int s);

    glm::vec3 Sampler(int x, int y) const;

    // Uses evenly spaced ray's in the pixel. Uses sample * sample.
    glm::vec3 UniformSampler(int x, int y) const;

    // Uses a random number generator in the [0,1] space to perturb original ray
    // sample x sample rays.
    glm::vec3 RandomSampler(int x, int y) const;
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
