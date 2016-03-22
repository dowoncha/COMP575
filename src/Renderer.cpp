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

	  for (size_t i = 0; i < bufferSize; ++i)
	  {
		    ColorBuffer.push_back(glm::vec3(0.0f));
		    DepthBuffer.push_back(0.0f);
	  }
}

void Renderer::Render()
{
    int fragCount = 0;

    for (int i = 0; i < scene.gNumTriangles; ++i)
    {
  		int base = 3 * i;

  		int k0 = scene.vIndexBuffer.at(base);
  		int k1 = scene.vIndexBuffer.at(base + 1);
  		int k2 = scene.vIndexBuffer.at(base + 2);

  		glm::vec4 v0 = scene.vertices.at(k0);
  		glm::vec4 v1 = scene.vertices.at(k1);
  		glm::vec4 v2 = scene.vertices.at(k2);

  		Barycentric bary(v0, v1, v2);
  		std::vector<glm::vec2> pixels = bary.GetInterior();

      if (pixels.empty())
        fragCount++;

  		for (glm::vec2 pixel : pixels)
  		{
  			ColorBuffer.at(pixel.x + pixel.y * ScreenWidth) = glm::vec3(1.0f);
      }
    }

    LOG(INFO) << "Empty frag count: " << fragCount;
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
	if (x < 0 || x > 512 || y < 0 || y > 512) return;

    ColorBuffer.at(y * ScreenWidth + x) = color;
}

void Renderer::ClearColorBuffer()
{
  for (auto& c : ColorBuffer)
  {
    c = glm::vec3(0.0f);
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
        LOG(ERROR) << "Image Buffer empty, please call SetBuffer before outputting. size: " << ColorBuffer.size();
        return;
    }
    if (ColorBuffer.size() != bufferSize)
    {
        LOG(WARNING) << "Buffer size is not equal to image width and height";
        return;
    }

    LOG(INFO) << "Outputting to ppm file: " << filename;

    // Open stream and write ppm headers, color bit's set to 255 per channel
    //std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    std::ofstream ofs(filename, std::ios::out);

    // write headers
    ofs << "P6"			  << '\n'
        << ScreenWidth    << ' '
        << ScreenHeight   << '\n'
        << 255			  << '\n';

    for (auto color : ColorBuffer)
    {
        glm::vec3 e = GammaEncode(color);
        Pixel p = ColorToPixel(e);

        ofs << Pixel_R(p)
            << Pixel_G(p)
            << Pixel_B(p);
    }

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
