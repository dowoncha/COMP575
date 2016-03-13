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
#include <cmath>
#include <cstdio>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Scene
{
public:
    glm::mat4x4 ModelTransform;
    glm::mat4x4 EyeTransform;
    glm::mat4x4 ViewTransform;
    glm::mat4x4 ProjTransform;
    glm::mat4x4 MVP;

    int ScreenWidth, ScreenHeight;

    int gNumVertices;        // # of vertices
    int gNumTriangles;       // # of triangles
    std::vector<int> vIndexBuffer;
    std::vector<glm::vec3> vertices;
    int* gIndexBuffer;

    Scene();

    ~Scene();

    void SetScreen(int width, int height);

    // Generate a sphere and load it into NumVertices, NumTriangles, IndexBuffer
    void LoadSphere();

    void SetupModelTransform(float scale, const glm::vec3& center);

    void SetupEyeTransform(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w, const glm::vec3& p);

    void SetupProjTransform(float l, float r, float b, float t, float n, float f);

    /**
     *  Setup up the viewport transform matrix
     *  @param nx X scaling factor?
     *  @param ny Y scaling factor?
     */
    void SetupViewportTransform(int nx, int ny);

    /**
     *  Calculate the Model-View-Projectiion matrix by multiplying all of the transforms
     */
    void SetupMVP();

    glm::mat4x4 GetMVP();
};

#endif // _RAST_SCENE_
