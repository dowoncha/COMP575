/*********************************************************************************
 *
 * filename  : RayTracer.cpp
 * author    : Do Won Cha
 * content   : This is a ray tracer to draw some spheres.
 *
 *********************************************************************************/

#ifdef _WIN32
    #define M_PI 3.14159265359
#elif __unix__ || __APPLE__
#endif

// Need to make this cross platfrom later on
#include <GL/gl3w.h>
//#include <GL/freeglut.h>
#include <iostream>
#include <cstring>

#include "easylogging++.h"
#include "RayTracer.h"
#include "Scene.h"
#include "Image.h"
#include "Primitives.h"

INITIALIZE_EASYLOGGINGPP

const char * elConf =
    "* Global:\n"
    "FORMAT = \"\%datetime \%msg\"\n"
    "FILENAME = \"./logs/my.log\"\n";
    //"To_Standard_Output = false";

static int width = 800, height = 600;

void RenderScene(void)
{
    //glClearColor(GL_COLOR_BUFFER_BIT);

    std::vector<Pixel> buffer = tracer.Render();

    // I can pass a vector here because the spec guarantees contiguous
    // space and pointer to the vector points to the memory.
}

void ResizeWindow(int w, int h)
{
    if (h == 0)  h = 1;

    float ratio = w * 1.0 / h;

    glViewport(0, 0, w, h);
}

int main(int argc, char *argv[])
{
    // Configure logging system
    el::Configurations conf;
    conf.setToDefault();
    conf.parseFromText(elConf);
    el::Loggers::reconfigureLogger("default", conf);

    // Check arguments
    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "-w") == 0)
        {
            width = std::stoi(argv[++i]);
        }
        else if (std::strcmp(argv[i], "-h") == 0)
        {
            height = std::stoi(argv[++i]);
        }
    }

    LOG(INFO) << "Ray tracer started, Width: " << width << ", Height: " << height;
    START_EASYLOGGINGPP(argc, argv);

    // DO THIS WHEN THERE IS TIME
    // Initialize glut and callbacks
    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Ray Tracer v0.0.1");

    // Define callbacks here
    glutDisplayFunc(RenderScene);
    glutReshapeFunc(ResizeWindow);
    glutIdleFunc(RenderScene);

    if (!gl3wInit()) {
        LOG(ERROR) << "Failed to initialize OpenGL";
        exit(EXIT_FAILURE);
    }
    if (!gl3wIsSupported(2, 0)) {
        LOG(ERROR) << "OpenGL 2.0 is not supported";
        exit(EXIT_FAILURE);
    }

    LOG(INFO) << "OpenGL " << glGetString(GL_VERSION)
              << ", GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION);

    exit(EXIT_SUCCESS);
}
