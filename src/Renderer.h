/*******************************************************************************
 *
 *	filename:    Renderer.h
 *	author:      Do Won Cha
 *	content:     GL Rendering class
 *
 ******************************************************************************/

#pragma once
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
// TODO: Add glm::normalize header


class Renderer
{
private:
    class Triangle
    {
    public:
      std::array<Vertex, 3> vertices;
    };

    class Vertex
    {
    public:
      glm::vec3 pos, color;
    }
public:
    /**
     *  Provide the output buffer and the width and height of the screen
     */
    Renderer(const Scene&);

    ~Renderer();

    void Initialize();

    void Run();
private:
    void Render();

    void DrawTopTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2);

    void DrawBotTriangle(const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2);

    void DrawRow(int x1, int x2, int scanlineY);

    glm::vec3 FragmentStage(const glm::vec3& color);

    glm::vec3 GammaEncode(const glm::vec3& color);

    static GPixel ColorToPixel(const glm::vec4& color);

    void ClearFrameBuffer();
    void ClearDepthBuffer();
private:
    const Scene scene;

    int ScreenWidth, ScreenHeight;

    // Buffers
    std::vector<glm::vec3> ColorBuffer;
    std::vecotr<float> DepthBuffer;

    size_t bufferSize;
    GLFWwindow* window;
};

#endif //_RENDERER_H_
