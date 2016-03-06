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
    // Deallocate frame and depth buffers here
    // delete[] FrameBuffer
    // delete[] DepthBuffer
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

    for (int triIndex = 0; triIndex < scene.gNumTriangles; ++i)
    {
        // Get triangle vertices fropm the index buffer
        int base = 3 * triIndex;

        int k0 = scene.gIndexBuffer[base];
        int k1 = scene.gIndexBuffer[base + 1];
        int k2 = scene.gIndexBuffer[base + 2];

        // Make barycentric coordinates?
        // Set pixels here
    }
}

void Rasterizer::ClearScreen(const glm::vec3& color)
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            FrameBuffer[x][y]
        }
    }
}
