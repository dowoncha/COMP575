/**
 *
 * 	filename   : Image.h
 *  author     : Do Won Cha
 *  content    : Container class to output a buffer to image.
 *
 */

#pragma once
#ifndef _RAY_IMAGE_
#define _RAY_IMAGE_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "easylogging++.h"
#include "Utility.h"

/**
 *  Define the output pixel as a 32 bit uint32_t. Currently in RGBA format
 *  will eventually add a enum for formats...
 */
using Pixel = uint32_t;

static Pixel MakePixel(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
{
    assert(r <= 255);
    assert(g <= 255);
    assert(b <= 255);
    assert(alpha <= 255);

    return  (r << 24) |
            (g << 16) |
            (b << 8)  |
            alpha;
}

static inline uint8_t GetPixelR(Pixel p) { return (p >> 24) & 0xff; }
static inline uint8_t GetPixelG(Pixel p) { return (p >> 16) & 0xff; }
static inline uint8_t GetPixelB(Pixel p) { return (p >> 8 ) & 0xff; }
static inline uint8_t GetPixelA(Pixel p) { return p & 0xff;         }

/**
 *	Image class, initialize with file output name, and with and height of the image.
 *  Make sure to SetBuffer before output call. Call OutputFORMAT(filename).
 */
class Image
{
public:

    enum ImageFormat
    {
      PPM
    };

    Image();
    Image(int width, int height);
    Image(std::string filename, int width, int height);

    ~Image();

    void SetBuffer(std::vector<glm::vec3> const & image);
    void SetFilename(std::string name = "default.ppm");

    static glm::vec3 GammaEncode (const glm::vec3& color);
    static glm::vec4 GammaEncode (const glm::vec4& color);

    static glm::vec3 PinToUnit(const glm::vec3& a);
    static glm::vec4 PinToUnit(const glm::vec4& a);

    static Pixel ColorToPixel(const glm::vec3& color);
    static Pixel ColorToPixel(const glm::vec4& color);

    void OutputPPM() const;
    void OutputPPM(std::string filename) const;
private:
    std::vector<glm::vec3> ImageBuffer;
    int Width, Height;
    std::string filename;
};

#endif /* end of include guard: _RAY_IMAGE_ */
