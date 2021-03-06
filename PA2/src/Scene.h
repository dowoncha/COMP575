/*******************************************************************************
 *
 *	filename:    Scene.h
 *	author:      Do Won Cha
 *	content:     Scene class
 *
 ******************************************************************************/

#pragma once
#ifndef _RAST_SCENE_
#define _RAST_SCENE_

#include <vector>
#include <cstdio>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/geometric.hpp>
#include <glm/ext.hpp>

#include "easylogging++.h"
#include "Vertex.hpp"
#include "Light.hpp"
#include "Utility.hpp"

class Vertex;

namespace Rasterizer
{

class Scene
{
private:
    glm::mat4x4 ModelTransform;
    glm::mat4x4 ViewTransform;
    glm::mat4x4 ProjTransform;
    glm::mat4x4 ViewportTransform;
    glm::mat4x4 MVP;
public:
    // TODO: Sphere Object members, cause too lazy to implement an Object class
    int gNumVertices;        // # of vertices
    int gNumTriangles;       // # of triangles
    std::vector<int> vIndexBuffer;
    std::vector<glm::vec4> vertices;
	  std::vector<Vertex> vVertices;

    Light light;
public:
    Scene();

    ~Scene();

    // Generate a sphere and load it into NumVertices, NumTriangles, IndexBuffer
    void LoadSphere();

    /**
     * Camera and matrix creations
     */
    void SetupModelTransform(const glm::vec3& scale, const glm::vec3& center);

    void SetupViewTransform(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w, const glm::vec3& p);

    void SetupProjTransform(float l, float r, float b, float t, float n, float f);

    /**
     *  Setup up the viewport transform matrix
     *  @param nx X scaling factor
     *  @param ny Y scaling factor
     */
    void SetupViewportTransform(int nx, int ny);

    void SetupMVP();

	  glm::mat4x4 ModelView() const;

	  glm::mat4x4 ProjViewport() const;

    glm::mat4x4 ModelViewProjection() const;

	  void NormalizeW(glm::vec4& v) const;
};

} // end of namespace Rasterizer

#endif // _RAST_SCENE_
