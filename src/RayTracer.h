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

    void Render(Image& image) const;

    Vector3f Shade(const Ray& ray, const HitData& Data) const;

    // Check whether shadow ray is blocked by any surface
    bool ShadowTrace(const HitData& Data) const;

    Vector3f CalculateLight(const Ray& ray, const HitData& Data, Light* light) const;

    enum PostProcess
    {
        UniformSampling,
        RandomSampling
    };

    void UniformSampler();

private:
    // Main scene to draw
    const Scene& mScene;

    Camera MainCamera;

    int ScreenWidth;
    int ScreenHeight;

    // These should be camera variables. Will use eventually
    float fov, angle;
    float AspectRatio;
};

#endif /* end of include guard: _RAY_TRACER_ */
