/*******************************************************************************
 *
 *	filename:    main.cpp
 *	author:      Do Won Cha
 *	content:     Start point for rasterizer
 *
 ******************************************************************************/

#include "Scene.h"
#include "Renderer.h"
#include "easylogging++.h"

const char* TITLE = "Comp 770 PA2 - Rasterizer";
const int SCREEN_WIDTH = 512, SCREEN_HEIGHT = 512;

INITIALIZE_EASYLOGGINGPP

int main(int argc, char* argv[])
{
    // Initialize logging
    START_EASYLOGGINGPP(argc, argv);

	// Initialize the Scene
    Rasterizer::Scene scene;

    scene.LoadSphere();
    scene.SetupModelTransform(glm::vec3(2.0f), glm::vec3(0.0f, 0.0f, -7.0f) );
    scene.SetupViewTransform(
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 0.0f)
    );

    scene.SetupProjTransform(-0.1f, 0.1f, -0.1f, 0.1f, -.1f, -1000.0f);
    scene.SetupViewportTransform(SCREEN_WIDTH, SCREEN_HEIGHT);
	scene.SetupMVP();

    // Initialize Renderer
	  Rasterizer::Renderer renderer(scene);
	  renderer.Initialize(argc, argv);
	     renderer.RenderUnshaded();
	   renderer.OutputToPPM("unshaded.ppm");

	   renderer.RenderFlat();
	   renderer.OutputToPPM("flatshading.ppm");

	   renderer.RenderGouraud();
       renderer.OutputToPPM("gouraudshading.ppm");

	  renderer.RenderPhong();
    renderer.OutputToPPM("phongshading.ppm");

    exit(EXIT_SUCCESS);
}
