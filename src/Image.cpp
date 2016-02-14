#include "Image.h"

Image::Image()
{
}

Image::Image(int width, int height) :
    Width(width),
    Height(height)
{
}

Image::~Image()
{
}

void Image::SetBuffer(const std::vector<Vector3f>& image)
{
    LOG(INFO) << "Image buffer set, buffer size is: " << image.size();

    ImageBuffer = image;
}

Vector3f Image::GammaEncode(const Vector3f& color)
{
    // For assignment, comment after turning in for sRGB encode function.
    float gamma = 1.0f / 2.2f;

    Vector3f out(
        std::pow(color.x, gamma),
        std::pow(color.y, gamma),
        std::pow(color.z, gamma)
    );

    /*
    float gamma = 1.0f / 2.4f;

    out.x = (color.x <= 0.0031308f ) ? 12.92 * color.x : 1.055 * std::pow(color.x, gamma) - 0.055;
    out.y = (color.y <= 0.0031308f ) ? 12.92 * color.y : 1.055 * std::pow(color.y, gamma) - 0.055;
    out.z = (color.z <= 0.0031308f ) ? 12.92 * color.z : 1.055 * std::pow(color.z, gamma) - 0.055;
    */

    return out;
}

Vector4f Image::GammaEncode(const Vector4f& color)
{
  // For assignment, comment after turning in for sRGB encode function.
  float gamma = 1.0f / 2.2f;

  Vector4f out(
      std::pow(color.x, gamma),
      std::pow(color.y, gamma),
      std::pow(color.z, gamma),
      std::pow(color.w, gamma)
  );

  /*
  float gamma = 1.0f / 2.4f;

  Vector4f out;

  out.x = (color.x <= 0.0031308f ) ? 12.92 * color.x : 1.055 * std::pow(color.x, gamma) - 0.055;
  out.y = (color.y <= 0.0031308f ) ? 12.92 * color.y : 1.055 * std::pow(color.y, gamma) - 0.055;
  out.z = (color.z <= 0.0031308f ) ? 12.92 * color.z : 1.055 * std::pow(color.z, gamma) - 0.055;
  out.w = (color.w <= 0.0031308f ) ? 12.92 * color.w : 1.055 * std::pow(color.z, gamma) - 0.055;
  */

  return out;
}

Vector3f Image::PinToUnit(const Vector3f& a)
{
    return Vector3f(
        Utility::PinToUnit(a.x),
        Utility::PinToUnit(a.y),
        Utility::PinToUnit(a.z)
    );
}

Vector4f Image::PinToUnit(const Vector4f& a)
{
    return Vector4f(
        Utility::PinToUnit(a.x),
        Utility::PinToUnit(a.y),
        Utility::PinToUnit(a.z),
        Utility::PinToUnit(a.w)
    );
}

Pixel Image::ColorToPixel(const Vector3f& color)
{
    //std::cout << normalized;
    Vector3f pinned = PinToUnit(color);

    uint8_t uR = (uint8_t) (pinned.x * 255.9999f);
    uint8_t uG = (uint8_t) (pinned.y * 255.9999f);
    uint8_t uB = (uint8_t) (pinned.z * 255.9999f);

    return MakePixel(uR, uG, uB, 255);
}

Pixel Image::ColorToPixel(const Vector4f& color)
{
    Vector4f pinned = PinToUnit(color);

    uint8_t uA = (uint8_t)(pinned.w * 255.9999f);
    uint8_t uR = (uint8_t)(pinned.x * uA);
    uint8_t uG = (uint8_t)(pinned.y * uA);
    uint8_t uB = (uint8_t)(pinned.z * uA);

    return MakePixel(uR, uG, uB, uA);
}

void Image::OutputPPM(std::string filename) const
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
    }

    LOG(INFO) << "Outputting to ppm file: " << filename;

    // Open stream and write ppm headers, color bit's set to 255 per channel
    std::ofstream ofs(filename, std::ios::out | std::ios::binary);

    // write headers
    ofs << "P6"     << '\n'
        << Width    << ' '
        << Height   << '\n'
        << 255      << '\n';

    for (Vector3f color : ImageBuffer)
    {
        Vector3f encoded = GammaEncode(color);
        Pixel p = ColorToPixel(color);

        ofs << GetPixelR(p)
            << GetPixelG(p)
            << GetPixelB(p);
    }

    ofs.close();
}
