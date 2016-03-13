#include "Rasterizer.h"

Rasterizer::Rasterizer(const Scene& scene) :
    scene(scene)
    width(scene.ScreenWidth),
    height(scene.ScreenHeight)
{
    FrameBuffer = new glm::vec3[width][height];
    DepthBuffer = new float[width][height];

    ClearDepthBuffer();
    ClearFrameBuffer();
}

Rasterizer::~Rasterizer()
{
    // Deallocate frame and depth buffers here
    //delete[] FrameBuffer
    // delete[] DepthBuffer
}

void Rasterizer::Pipeline()
{
  for (int i = 0; i < scene.gNumTriangles; ++i)
  {
    // Get triangle vertices fropm the index buffer
    int base = 3 * triIndex;

    //int k0 = scene.gIndexBuffer[base];
    //int k1 = scene.gIndexBuffer[base + 1];
    //int k2 = scene.gIndexBuffer[base + 2];

    // Get the vertices index
    int k0 = scene.vIndexBuffer.at(base);
    int k1 = scene.vIndexBuffer.at(base + 1);
    int k2 = scene.vIndexbuffer.at(base + 2);

    Triangle tri;

    // Process each vertex of the triangle
    tri.v0 = VertexStage(scene.vertices.at(k0));
    tri.v1 = VertexStage(scene.vertices.at(k1));
    tri.v2 = VertexStage(scene.vertices.at(k2));

    Rasterize(tri);

    FragmentStage();

    WriteToFrameBuffer();
  }
}

glm::vec3 Rasterizer::VertexStage(const glm::vec3& vertex)
{
  // Transform each vertex by the MVP
  return scene.MVP * vertex;
}

void Rasterizer::Rasterize(const Triangle& tri)
{
  // Rasterization using Bresenham Algorithm
  // First sort the vertices by y and partition into 2 top and bottom triangles
  std::sort(tri.verts, [] (const glm::vec2& a, const glm::vec2& b) => bool {
    return a.y < b.y;
  });

  //TODO: Sort the colors

  // Neccessary Case: If bottom vertices are on the same row just calculate top
  if (v1.y == v2.y)
  {
    DrawTopTriangle(v0, v1, v2);
  }
  // Necessary Case: If top 2 vertices are on the same row draw bottom
  else if ( v1.y == v2.y)
  {
    DrawBotTriangle(v0, v1, v2);
  }
  else
  {
    //Partition the triangle horizontally into 2 halves
    //Vertex of the partitioning point
    glm::vec2 g;
    DrawTopTriangle(v0, v1, g);
    DawBotTriangle(g, v1, v2);
  }
}

void Rasterizer::DrawTopTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
  float invSlope1 = (v1.x - v0.x) / (v1.y - v0.y);
  float invSlope2 = (v2.x - v0.x) / (v2.y - v0.y);

  // Calculate the inverse slope here
  int currentX1 = v1.x;
  int currentX2 = v2.x;
  for (int y = v0.y; y < v1.y; ++y)
  {
    DrawRow(currentX1, currentX2, y);
    currentX1 += invSlope1;
    currentX2 += invSlope2;
  }
}

void Rasterizer::DrawBotTriangle(const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2)
{
  // Calculate inverse slope here
  float invSlope1 = (v2.x - v0.x) / (v2.y - v0.y);
  float invSlope2 = (v2.x - v1.x) / (v2.y - v1.y);

  float currentX1 = v0.x;
  float currentX2 = v1.x;

  for (int y = v0.y; y < v2.y; ++y)
  {
    // Draw Row
    DrawRow(currentX1, currentX2, y);
    currentX1 += invSlope1;
    currentX2 += invSlope2;
  }
}

void Rasterizer::DrawRow(int x1, int x2, int scanlineY)
{
  for (int i = x1; i < x2; ++i)
  {
    FrameBuffer[i][scanlineY] = FragmentStage(glm::ivec3(255, 255, 255));
  }
}

glm::vec3 Rasterizer::FragmentStage(const glm::vec3& color)
{
  return glm::vec3(1.0f);
}

void Rasterizer::ClearFrameBuffer()
{
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            FrameBuffer[x][y] = glm::vec3(0);
        }
    }
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
