/*******************************************************************************
 *
 *	filename:    Rasterizer.h
 *	author:      Do Won Cha
 *	content:     Rasterizer class to rasterize the given scene
 *
 ******************************************************************************/

#pragma once
#define _RAST_ERIZER_
#ifndef _RAST_ERIZER_

#include <glm::vec3.hpp>

#include "Scene.h"

class Rasterizer
{
private:
    const Scene& scene;
    int width, height;

    // Buffers
    glm::vec3[][] FrameBuffer;
    float[][] DepthBuffer;
public:
    Rasterizer(const Scene& scene);
    ~Rasterizer();

    void Rasterize();

    void ClearScreen(const glm::vec3& color);
};

#endif // _RAST_ERIZER_
