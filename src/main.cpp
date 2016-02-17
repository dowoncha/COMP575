/*********************************************************************************
 *
 * filename  : RayTracer.cpp
 * author    : Do Won Cha
 * content   : This is a ray tracer to draw some spheres.
 *
 *********************************************************************************/

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

    Scene scene;

    // Create necessary scene, image, and ray tracer objects
    Material white = Material::CreateWhiteMat();
    Material red = Material::CreateRedMat();
    Material green = Material::CreateGreenMat();
    Material blue = Material::CreateBlueMat();

    Sphere sphere1( Vector3f(0.0f, 0.0f, -7.0f), 2.0f, green);
    Sphere sphere2( Vector3f(-4.0f, 0.0f, -7.0f), 1.0f, red);
    Sphere sphere3( Vector3f(4.0f, 0.0f, -7.0f), 1.0f, blue);

    Plane plane(Vector3f(0.0f, -2.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), white);

    Light light1(Vector3f(-4.0f, 4.0f, -3.0f), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(1.0f, 1.0f, 1.0f));
    light1.SetIntensity(1.0f);

    scene.AddLight(&light1);
    scene.AddSurface(&sphere1);

    Image image(width, height);
    RayTracer rTracer(scene, width, height);

    rTracer.BWRender(image);
    image.OutputPPM("bw.ppm");

    scene.Surfaces.push_front(&plane);
    scene.AddSurface(&sphere2);
    scene.AddSurface(&sphere3);

    // render image with no anti-aliasing
    rTracer.Render(image);
    image.OutputPPM("no-aa.ppm");

    // Render with 8x8 sample rate uniform samling
    rTracer.SetSampleRate(8);
    rTracer.Render(image);
    image.OutputPPM("8x8uniformsampling.ppm");

    // Render 8x8 anti-aliase random sampling.
    rTracer.SamplingType = RayTracer::PostProcess::RandomSampling;
    rTracer.Render(image);
    image.OutputPPM("8x8randomsampling.ppm");

    exit(EXIT_SUCCESS);
}
