#include "Renderer.h"

Renderer::Renderer(const Scene& _scene) :
    scene(_scene),
    window(nullptr)
{
}

Renderer::~Renderer()
{
    // Destroy GLFW window and context
}

void Renderer::Initialize(int width, int height)
{
  if (!glfwInit())
  {
    return -1;
  }

  window = glfwCreateWindow(scene.ScreenWidth, scene.ScreenHeight, "title", nullptr, nullptr);

  glfwMakeContextCurrent(window);

  ScreenWidth = width;
  ScreenHeight = height;

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

  // TODO: reisze window and such,probably scene as well
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
  std::sort(tri.vertices, [] (const Vertex& a, const Vertex& b) -> bool {
    return a.vertex.y < b.vertex.y;
  });

  // Neccessary Case: If bottom vertices are on the same row just calculate top
  if (tri.vertices[1].y == tri.vertices[2].y)
  {
    DrawTopTriangle(tri.vertices[0], tri.vertices[1], tri.vertices[2]);
  }
  // Necessary Case: If top 2 vertices are on the same row draw bottom
  else if ( v0.y == v1.y)
  {
    DrawBotTriangle(tri.vertices[0], tri.vertices[1], tri.vertices[2]);
  }
  else
  {
    //Partition the triangle horizontally into 2 halves
    //Vertex of the partitioning point
    //glm::vec2 g;
    DrawTopTriangle(v0, v1, g);
    DawBotTriangle(g, v1, v2);
  }
}

void Renderer::DrawTopTriangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
  // Calculate the inverse slope here
  float invSlope1 = (v1.x - v0.x) / (v1.y - v0.y);
  float invSlope2 = (v2.x - v0.x) / (v2.y - v0.y);

  float currentX1 = v1.x;
  float currentX2 = v2.x;
  for (float y = v0.y; y < v1.y; ++y)
  {
    DrawRow(currentX1, currentX2, y);
    currentX1 += invSlope1;
    currentX2 += invSlope2;
  }
}

void Renderer::DrawBotTriangle(const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2)
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
    //TODO: change at to [], cause fuck range check safety
    DepthBuffer.at(i) = 0.0f;
  }
}

GPixel Renderer::ColorToPixel(const glm::vec4& color)
{
  // TODO: Temporarily using normalize, but this is potentially incorrect
  // each x,w,y,z value needs to be pinned between 0-1, not normalized
  auto pinned = glm::normalize(color);

  float fA =  pinned.a * 255.9999f;		  //Convert from 0-1 to 0-255
  uint8_t uA = (uint8_t) fA;
  uint8_t uR = (uint8_t) (pinned.r * fA);  //Multiply rgb values by the new alpha
  uint8_t uG = (uint8_t) (pinned.g * fA);
  uint8_t uB = (uint8_t) (pinned.b * fA);

  return GPixel_PackARGB(uA, uR, uG, uB);	  //Returned the packed pixel
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
