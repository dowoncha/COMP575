/*******************************************************************************
 *
 *	filename:    Rasterizer.h
 *	author:      Do Won Cha
 *	content:     Rasterizer class to rasterize the given scene
 *
 ******************************************************************************/

#pragma once
#define _RASTERIZER_H_
#ifndef _RASTERIZER_H_

#include <glm/vec3.hpp>
#include <glm/ivec3.hpp>

#include "Scene.h"

class Rasterizer
{
  class Triangle
  {
  public:
    // Vertices
    std::array<glm::vec3, 3> verts;
    //Vertex colors
    std::array<glm::vec3, 3> colors;
  };
private:
    const Scene& scene;
    int width, height;

    // Buffers
    glm::ivec3[][] FrameBuffer;
    float[][] DepthBuffer;
public:
    Rasterizer(const Scene& scene);

    ~Rasterizer();

    void Pipeline();

    void Transform();
private:
    /**
     *  Takes a a vertex to transform from objectspace to screenspace
     *  Also calculate lighting here
     *  @param  position Vertex position
     *  @return          The output color for the vertex
     */
    void VertexStage(const glm::vec3& position);

    /**
     *  Convert primitives (triangles) into pixels
     */
    void Rasterize(const Triangle& tri);

    void DrawTopTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);
    void DrawBotTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

    /**
     *  "Pixel shader", Blending, compositing (depth-buffer), shading,
     *  Final write to FrameBuffer
     */
    void FragmentStage(glm::vec3& pixel);

    void ClearFrameBuffer();
    void ClearDepthBuffer();
};

#endif // _RASTERIZER_H_
