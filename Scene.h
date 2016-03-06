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

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

class Scene
{
public:
    glm::mat4x4 ModelTransform;
    glm::mat4x4 EyeTransform;
    glm::mat4x4 ViewTransform;
    glm::mat4x4 ProjTransform;

    int ScreenWidth, ScreenHeight;
    int gNumVertices;        // # of vertices
    int gNumTriangles;       // # of triangles
    int* gIndexBuffer;

    Scene();
    ~Scene();

    void LoadSphere();

    void SetupModelTransform(float scale, const glm::vec3& center);

    void SetupEyeTransform(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w, const glm::vec3& p);

    void SetupProjTransform(float l, float r, float b, float t, float n, float f);

    void SetupViewportTransform(int nx, int ny);
};

#endif // _RAST_SCENE_
