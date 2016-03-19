/*******************************************************************************
 *
 *	filename:    main.cpp
 *	author:      Do Won Cha
 *	content:     Start point for rasterizer
 *
 ******************************************************************************/

#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "Scene.h"
#include "Rasterizer.h"
#include "easylogging++.h"

static const char* TITLE = "Comp 770 PA2 - Rasterizer";
static const int SCREEN_WIDTH = 512, SCREEN_HEIGHT = 512;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{
    // parse arguments here
    START_EASYLOGGINGPP(argc, argv);

    // Initialize the Scene
    LOG(INFO) << "Initializing Scene";
    Scene scene;

    scene.SetScreen(512, 512);
    scene.LoadSphere();
    scene.SetupModelTransform(2, glm::vec3(0.0f, 0.0f, -7.0f) );
    scene.SetupEyeTransform(
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    scene.SetupProjTransform(-0.1f, 0.1f, -0.1f, -0.1f, -1000.0f);
    scene.SetupViewportTransform(SCREEN_WIDTH, SCREEN_HEIGHT);
    scene.SetupMVP();
    scene.ApplyAllTransforms();

    // Render Output code, GL CODE
    Renderer renderer(scene);
    renderer.Initialize();
    renderer.Render();

    exit(EXIT_SUCCESS);
}
