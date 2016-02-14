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

#include <GL/freeglut.h>
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
    "FILENAME = \"./logs/my.log\"\n"
    "To_Standard_Output = false";

int main(int argc, char *argv[])
{
    // Configure logging system
    el::Configurations conf;
    conf.setToDefault();
    conf.parseFromText(elConf);
    el::Loggers::reconfigureLogger("default", conf);

    int width = 800, height = 600;

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

    // Initialize glut and callbacks
    glutInit(&argc, argv);
    glutInitWindowSize(width, height);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL);

    // Setup up logging when time
    //glutInitErrorFunc(LogError);

    // Create necessary scene, image, and ray tracer objects
    Scene scene;
    Image image(width, height);

    RayTracer rTracer(scene, width, height);

    // Render and output the resulting image
    rTracer.Render(image);
    image.OutputPPM("test1.ppm");

    exit(EXIT_SUCCESS);
}
