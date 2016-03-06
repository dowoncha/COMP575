#include "Rasterizer.h";

Rasterizer::Rasterizer(const Scene& scene) :
    scene(scene)
    width(scene.ScreenWidth),
    height(scene.ScreenHeight)
{
    FrameBuffer = new glm::vec3[width][height];
    DepthBuffer = new float[width][height];

    ClearDepthBuffer();
    ClearScreen(glm::vec3(0.0f, 0.0f, 0.0f));
}

Rasterizer::~Rasterizer()
{

}

void Rasterizer::ClearDepthBuffer()
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            DepthBuffer[y][x] = -100000.0f;
        }
    }
}

void Rasterizer::Rasterize()
{
    ClearScreen();
}

void Rasterizer::ClearScreen(const glm::vec3& color)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            //Buffer = color;
        }
    }
}
