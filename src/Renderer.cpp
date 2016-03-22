#include "Renderer.h"

using namespace Rasterizer;

Renderer::Renderer(const Scene& _scene) :
    scene(_scene),
    ScreenWidth(512),
    ScreenHeight(512)
{
}

Renderer::~Renderer()
{
    // Destroy GLFW window and context
}

void Renderer::Initialize(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
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

    BufferInit();
}

void Renderer::BufferInit()
{
    bufferSize = ScreenWidth * ScreenHeight;

    LOG(INFO) << "Buffer size is: " << bufferSize;

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

void Renderer::Render()
{
    for (int i = 0; i < scene.gNumTriangles; ++i)
    {
      int base = 3 * i;

      //Model View Transformation
      //Triangle tri;
      //tri.vertices[0].pos = scene.ModelView() * scene.vertices[base];
      //tri.vertices[1].pos = scene.ModelView() * scene.vertices[base + 1];
      //tri.vertices[2].pos = scene.ModelView() * scene.vertices[base + 2];
      // Vertex Shader
      // Shading

      // Projection Transformation
      //tri.vertices[0].pos = scene.Projection() * scene.vertices[base];
      //tri.vertices[1].pos = scene.Projection() * scene.vertices[base + 1];
      //tri.vertices[2].pos = scene.Projection() * scene.vertices[base + 2];

      //auto v0 = scene.ModelViewProj() * scene.vertices[base];
      //auto v1 = scene.ModelViewProj() * scene.vertices[base + 1];
      //auto v2 = scene.ModelViewProj() * scene.vertices[base + 2];

      Barycentric(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.0f));
    }
}

void Renderer::Barycentric(const glm::vec3& v0,
                           const glm::vec3& v1,
                           const glm::vec3& v2 )
{
  int maxX = std::max(v0.x, std::max(v1.x, v2.x));
  int minX = std::min(v0.x, std::min(v1.x, v2.x));
  int maxY = std::max(v0.y, std::max(v1.y, v2.y));
  int minY = std::min(v0.y, std::min(v1.y, v2.y));

  glm::vec3 vs1(v1.x - v0.x, v1.y - v0.y, 1.0f);
  glm::vec3 vs2(v2.x - v0.x, v2.y - v0.y, 1.0f);

  for (int y = minY; y <= maxY; ++y)
  {
    for (int x = minX; x <= maxX; ++x)
    {
      glm::vec3 q(x - v0.x, y - v1.y, 1.0f);

      float s = glm::length(glm::cross(q, vs2)) / glm::length(glm::cross(vs1, vs2));
      float t = glm::length(glm::cross(vs1, q)) / glm::length(glm::cross(vs1, vs2));

      if ((s >= 0) && (t >= 0) && (s + t <= 1))
      {
        //Inside triangle
        DrawPixel(x, y, glm::vec3(1.0f));
      }
    }
  }
}

void Renderer::DrawRow(int x1, int x2, int y)
{
  int rowOffset = y * ScreenWidth;
  for (int i = x1; i < x2; ++i)
  {
    //FrameBuffer[i][scanlineY] = FragmentStage(glm::ivec3(255, 255, 255));
    ColorBuffer.at(rowOffset + i) = glm::vec3(1.0f);
  }
}

void Renderer::DrawPixel(int x, int y, const glm::vec3& color)
{
    ColorBuffer.at(y * ScreenWidth + x) = color;
}

void Renderer::ClearColorBuffer()
{
  for (auto& c : ColorBuffer)
  {
    c = glm::vec3(1.0f);
  }
}

void Renderer::ClearDepthBuffer()
{
  for (auto& d : DepthBuffer)
  {
    d = 0.0f;
  }
}

void Renderer::OutputToPPM(const std::string& filename) const
{
    if (ColorBuffer.empty())
    {
        LOG(ERROR) << "Image Buffer empty, please call SetBuffer before outputting";
        return;
    }
    if (ColorBuffer.size() != bufferSize)
    {
        LOG(WARNING) << "Buffer size is not equal to image width and height";
        return;
    }

    LOG(INFO) << "Outputting to ppm file: " << filename;

    // Open stream and write ppm headers, color bit's set to 255 per channel
    std::ofstream ofs(filename, std::ios::out | std::ios::binary);

    // write headers
    ofs << "P6"     << '\n'
        << ScreenWidth    << ' '
        << ScreenHeight   << '\n'
        << 255      << '\n';

    for (glm::vec3 color : ColorBuffer)
    {
        glm::vec3 encoded = GammaEncode(color);
        Pixel p = ColorToPixel(color);

        ofs << Pixel_R(p)
            << Pixel_G(p)
            << Pixel_B(p);
    }

    ofs.close();

    LOG(INFO) << "Finished outputting to " << filename << ", closing file.";
}

glm::vec3 Renderer::GammaEncode(const glm::vec3& color) const
{
    float gamma = 1.0f / 2.4f;

    glm::vec3 out;

    out.x = (color.x <= 0.0031308f ) ? 12.92f * color.x : 1.055f * std::pow(color.x, gamma) - 0.055f;
    out.y = (color.y <= 0.0031308f ) ? 12.92f * color.y : 1.055f * std::pow(color.y, gamma) - 0.055f;
    out.z = (color.z <= 0.0031308f ) ? 12.92f * color.z : 1.055f * std::pow(color.z, gamma) - 0.055f;

    return out;
}

glm::vec3 Renderer::PinToUnit(const glm::vec3& color) const
{
  float x = std::max(0.0f, std::min(1.0f, color.x));
  float y = std::max(0.0f, std::min(1.0f, color.y));
  float z = std::max(0.0f, std::min(1.0f, color.z));

  return glm::vec3(x, y, z);
}

Pixel Renderer::ColorToPixel(const glm::vec3& color) const
{
    glm::vec3 pinned = PinToUnit(color);

    uint8_t uR = (uint8_t) (pinned.x * 255.9999f);
    uint8_t uG = (uint8_t) (pinned.y * 255.9999f);
    uint8_t uB = (uint8_t) (pinned.z * 255.9999f);

    return MakeRGBA(uR, uG, uB, 255);
}

void Renderer::linEval(int xl, int xr, int yl, int yr, const glm::vec3& color)
{
  float qRow = color.x * xl + color.y * yl + color.z;

  for (int y = yl; y < yr; ++y)
  {
    float qPix = qRow;
    for (int x = xl; x < xr; ++x)
    {
      DrawPixel(x, y, glm::vec3(1.0f));
      qPix += color.x;
    }
    qRow += color.y;
  }
}
