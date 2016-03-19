#include "Renderer.h"

using namespace Rasterizer;

Renderer::Renderer(const Scene& _scene) :
    scene(_scene),
    ScreenWidth(512),
    ScreenHeight(512),
    window(nullptr)
{
}

Renderer::~Renderer()
{
    // Destroy GLFW window and context
}

void Renderer::Initialize(int argc, char* argv[])
{
    for (int i = 1; i < *argc; ++i)
    {
        if (std::strcmp(argv[i], "--width") == 0)
        {
            ScreenWidth = atoi(argv[++i]);
        }
        else if (std::strcmp(argv[i], "--height") == 0)
        {
            ScreenHeight = atoi(argv[++i]);
        }
    }

    GLInit();
    BufferInit();
}

void Renderer::GLInit()
{
    if (!glfwInit())
    {
        LOG(ERROR) << "Failed to initialize GLFW";

        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(ScreenWidth, ScreenHeight, "title", nullptr, nullptr);

    glfwMakeContextCurrent(window);

    if (gl3wInit())
    {
        LOG(ERROR) << "Failed to initialize OpenGL\n";
        exit(EXIT_FAILURE);
    }

    if (!gl3wIsSupported(2, 0))
    {
        LOG(ERROR) << "OpenGL 2.0 is not supported";
        exit(EXIT_FAILURE);
    }
}

void Renderer::BufferInit()
{
    bufferSize = ScreenWidth * ScreenHeight;

    ColorBuffer.reserve(bufferSize);
    DepthBuffer.reserve(bufferSize);

    ClearColorBuffer();
    ClearDepthBuffer();
}

void Renderer::Resize(int width, int height)
{
  ScreenWidth = width;
  ScreenHeight = height;

  bufferSize = width * height;

  ColorBuffer.resize(bufferSize);
  DepthBuffer.resize(bufferSize);

  ClearColorBuffer();
  ClearDepthBuffer();

  // TODO: resize glfw window and such, probably scene as well
}

void Renderer::Run()
{
  // Render the scene sphere into the FrameBuffer
  Render();

  // Main loop
  while(!glfwWindowShouldClose(window))
  {
    // Update
    // If this wasn't on CPU put login in here

    glClear(GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT);

    // Draw the FrameBuffer
    glDrawPixels(
      SCREEN_WIDTH,     // width
      SCREEN_HEIGHT,    // height
      GL_RGB,          // Format
      GL_FLOAT,         // Type
      &ColorBuffer[0]   // buffer ptr
    );
    // Swap Front and back buffers
    glfwSwapBuffers(window);
    // Poll for and process events
    glfwPollEvents();
  }

  glfwTerminate();
}

void Renderer::Render()
{
    // Vertex Shader
    std::vector<Triangle> triangles(scene.gNumTriangles);

    for (int i = 0; i < scene.gNumTriangles; ++i)
    {
      int base = 3 * triIndex;

      //Model View Transformation
      Triangle tri;
      tri.vertices[0] = scene.ModelView() * scene.vertices[base];
      tri.vertices[1] = scene.ModelView() * scene.vertices[base + 1];
      tri.vertices[2] = scene.ModelView() * scene.vertices[base + 2];
      // Vertex Shader
      // Shading

      // Projection Transformation
      tri.vertices[0] = scene.Projection() * scene.vertices[base];
      tri.vertices[1] = scene.Projection() * scene.vertices[base + 1];
      tri.vertices[2] = scene.Projection() * scene.vertices[base + 2];

      // Screen Mapping
    }

    // Triangle Setup
    // Triangle Traversal
    for (const Triangle& tri : triangles)
    {
      Rasterize(tri);
      // Pixel Shader
      // Merge && Depth Buffer Test
    }
}

void Renderer::Rasterize(const Triangle& tri)
{
  // Rasterization using Bresenham Algorithm
  // First sort the vertices by y and partition into 2 top and bottom triangles
  std::sort(tri.vertices.begin(), tri.vertices.end(), [] (const Vertex& a, const Vertex& b) -> bool {
    return a.pos.y < b.pos.y;
  });

  // Neccessary Case: If bottom vertices are on the same row just calculate top
  if (tri.vertices[1].pos.y == tri.vertices[2].pos.y)
  {
    DrawTopTriangle(tri.vertices[0], tri.vertices[1], tri.vertices[2]);
  }
  // Necessary Case: If top 2 vertices are on the same row draw bottom
  else if ( tri.vertices[0].pos.y == tri.vertices[1].pos.y)
  {
    DrawBotTriangle(tri.vertices[0], tri.vertices[1], tri.vertices[2]);
  }
  else
  {
    //Partition the triangle horizontally into 2 halves
    //Vertex of the partitioning point
    Vertex g;
    DrawTopTriangle(tri.vertices[0], tri.vertices[1], g);
    DrawBotTriangle(g, tri.vertices[1], tri.vertices[2], g);
  }
}

void Renderer::DrawTopTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
{
  // Calculate the inverse slope here
  float invSlope1 = (v1.pos.x - v0.pos.x) / (v1.pos.y - v0.pos.y);
  float invSlope2 = (v2.pos.x - v0.pos.x) / (v2.pos.y - v0.pos.y);

  float currentX1 = v1.pos.x;
  float currentX2 = v2.pos.x;
  for (float y = v0.pos.y; y < v1.pos.y; ++y)
  {
    DrawRow(currentX1, currentX2, y);
    currentX1 += invSlope1;
    currentX2 += invSlope2;
  }
}

void Renderer::DrawBotTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
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

void Renderer::DrawRow(int x1, int x2, int scanlineY)
{
  for (int i = x1; i < x2; ++i)
  {
    FrameBuffer[i][scanlineY] = FragmentStage(glm::ivec3(255, 255, 255));
  }
}

glm::vec3 Renderer::FragmentStage(const glm::vec3& color)
{
  return glm::vec3(1.0f);
}

void Renderer::ClearColorBuffer()
{
  for (int i = 0; i < bufferSize; ++i)
  {
      ColorBuffer.at(i) = glm::vec3(0.0f);
  }
}

void Renderer::ClearDepthBuffer()
{
  for (int i = 0; i < bufferSize; ++i )
  {
      DepthBuffer.at(i) = 0.0f;
  }
}

glm::vec3 Renderer::GammaEncode(const glm::vec3& color)
{
    float gamma = 1.0f / 2.4f;

    glm::vec3 out;

    out.x = (color.x <= 0.0031308f ) ? 12.92f * color.x : 1.055f * std::pow(color.x, gamma) - 0.055f;
    out.y = (color.y <= 0.0031308f ) ? 12.92f * color.y : 1.055f * std::pow(color.y, gamma) - 0.055f;
    out.z = (color.z <= 0.0031308f ) ? 12.92f * color.z : 1.055f * std::pow(color.z, gamma) - 0.055f;

    return out;
}
