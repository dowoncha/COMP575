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

#include <glm/vec3.hpp>

class Renderer
{
public:
    glm::vec3 **FrameBuffer;
    int ScreenWidth, ScreenHeight;

    /**
     *  Provide the output buffer and the width and height of the screen
     */
    Renderer(glm::vec3 **buffer, int width, int height);
    ~Renderer();

    void Init();

    void Render();
};

#endif //_RENDERER_H_
