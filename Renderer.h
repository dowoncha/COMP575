/*******************************************************************************
 *
 *	filename:    Renderer.h
 *	author:      Do Won Cha
 *	content:     GL Rendering class
 *
 ******************************************************************************/

#pragma once
#ifndef _RAST_RENDERER_
#define _RAST_RENDERER_

class Renderer
{
public:
    glm::vec3 **FrameBuffer;
    int ScreenWidth, ScreenHeight;

    Renderer(glm::vec3 **buffer, int width, int height);

    ~Renderer();

    void Init();

    void Render();
};

#endif _RAST_RENDERER_
