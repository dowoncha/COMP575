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
    std::vector<glm::vec3> vertices;
    int* gIndexBuffer;
public:
    Scene();

    ~Scene();

    // Generate a sphere and load it into NumVertices, NumTriangles, IndexBuffer
    void LoadSphere();

    void SetupModelTransform(float scale, const glm::vec3& center);

    void SetupViewTransform(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w, const glm::vec3& p);

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

    glm::mat4x4 ModelViewProj() const;

    glm::mat4x4 ModelView() const;
};

#endif // _RAST_SCENE_
