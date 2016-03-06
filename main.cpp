/*******************************************************************************
 *
 *	filename:    main.cpp
 *	author:      Do Won Cha
 *	content:     Start point for rasterizer
 *
 ******************************************************************************/

#include <glm/vec3.hpp>

#include "Scene.h"
#include "Rasterizer.h"

static const char* TITLE = "Comp 770 PA2 - Rasterizer";
static const int SCREEN_WIDTH = 512, SCREEN_HEIGHT = 512;

int main(int argc, char* argv[])
{
    // parse arguments here

    Scene scene();

    scene.LoadSphere();
    scene.SetupModelTransform(2, glm::vec3(0.0f, 0.0f, -7.0f) );
    
    //scene.SetupEyeTransform();
    //scene.SetupProjTransform(-0.1f, 0.1f, -0.1f, -0.1f, -1000f);
    //scene.SetupViewportTransform(SCREEN_WIDTH, SCREEN_HEIGHT);
    //scene.SetupCompTransform
    //scene.ApplyAllTransform

    // Rasterizer r_a(scene);
    // r_a.rasterize();

    // Render Output code, GL CODE
    // Renderer render(r_a.FrameBuffer, 512, 512);
}
