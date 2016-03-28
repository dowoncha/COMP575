#include "Image.h"

Image::Image()
{
}

Image::Image(int width, int height) :
    filename("default.ppm"),
    Width(width),
    Height(height)
{
}

Image::Image(std::string file, int width, int height) :
    filename(file),
    Width(width),
    Height(height)
{
}

Image::~Image()
{
}

void Image::SetBuffer(std::vector<glm::vec3> const & image)
{
    LOG(INFO) << "Image buffer set, buffer size is: " << image.size();
    ImageBuffer = image;
}

void Image::SetFilename(std::string name)
{
    LOG(INFO) << "Image name set to: " << name;
    filename = name;
}

glm::vec3 Image::GammaEncode(glm::vec3 const & color)
{
    float gamma = 1.0f / 2.4f;

    glm::vec3 out;

    out.x = (color.x <= 0.0031308f ) ? 12.92 * color.x : 1.055 * std::pow(color.x, gamma) - 0.055;
    out.y = (color.y <= 0.0031308f ) ? 12.92 * color.y : 1.055 * std::pow(color.y, gamma) - 0.055;
    out.z = (color.z <= 0.0031308f ) ? 12.92 * color.z : 1.055 * std::pow(color.z, gamma) - 0.055;

    return out;
}

glm::vec4 Image::GammaEncode(glm::vec4 const & color)
{
  float gamma = 1.0f / 2.4f;

  glm::vec4 out;

  out.x = (color.x <= 0.0031308f ) ? 12.92 * color.x : 1.055 * std::pow(color.x, gamma) - 0.055;
  out.y = (color.y <= 0.0031308f ) ? 12.92 * color.y : 1.055 * std::pow(color.y, gamma) - 0.055;
  out.z = (color.z <= 0.0031308f ) ? 12.92 * color.z : 1.055 * std::pow(color.z, gamma) - 0.055;
  out.w = (color.w <= 0.0031308f ) ? 12.92 * color.w : 1.055 * std::pow(color.z, gamma) - 0.055;

  return out;
}

glm::vec3 Image::PinToUnit(glm::vec3 const & a)
{
    return glm::vec3(
        Utility::PinToUnit(a.x),
        Utility::PinToUnit(a.y),
        Utility::PinToUnit(a.z)
    );
}

glm::vec4 Image::PinToUnit(glm::vec4 const & a)
{
    return glm::vec4(
        Utility::PinToUnit(a.x),
        Utility::PinToUnit(a.y),
        Utility::PinToUnit(a.z),
        Utility::PinToUnit(a.w)
    );
}

Pixel Image::ColorToPixel(glm::vec3 const & color)
{
    glm::vec3 pinned = PinToUnit(color);

    uint8_t uR = (uint8_t) (pinned.x * 255.9999f);
    uint8_t uG = (uint8_t) (pinned.y * 255.9999f);
    uint8_t uB = (uint8_t) (pinned.z * 255.9999f);

    return MakePixel(uR, uG, uB, 255);
}

Pixel Image::ColorToPixel(glm::vec4 const & color)
{
    glm::vec4 pinned = PinToUnit(color);

    uint8_t uA = (uint8_t)(pinned.w * 255.9999f);
    uint8_t uR = (uint8_t)(pinned.x * uA);
    uint8_t uG = (uint8_t)(pinned.y * uA);
    uint8_t uB = (uint8_t)(pinned.z * uA);

    return MakePixel(uR, uG, uB, uA);
}

void Image::OutputPPM(std::string file) const
{
    if (ImageBuffer.empty())
    {
        LOG(ERROR) << "Image Buffer empty, please call SetBuffer before outputting";
        return;
    }
    if (ImageBuffer.size() != Width * Height)
    {
        LOG(WARNING) << "Buffer size is not equal to image width and height. Still outputting";
        LOG(WARNING) << "Buffer size: " << ImageBuffer.size() << ", Width: " << Width << ", Height: " << Height;
        return;
    }

    LOG(INFO) << "Outputting to ppm file: " << file;

    // Open stream and write ppm headers, color bit's set to 255 per channel
    std::ofstream ofs(file, std::ios::out | std::ios::binary);

    // write headers
    ofs << "P6"     << '\n'
        << Width    << ' '
        << Height   << '\n'
        << 255      << '\n';

    for (glm::vec3 color : ImageBuffer)
    {
        glm::vec3 encoded = GammaEncode(color);
        Pixel p = ColorToPixel(color);

        ofs << GetPixelR(p)
            << GetPixelG(p)
            << GetPixelB(p);
    }

    ofs.close();

    LOG(INFO) << "Finished outputting to " << file << ", closing file.";
}
