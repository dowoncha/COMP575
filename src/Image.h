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

#include "easylogging++.h"
#include "Primitives.h"
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

    ~Image();

    void SetBuffer(const std::vector<Vector3f>& image);

    static Vector3f GammaEncode (const Vector3f& color);
    static Vector4f GammaEncode (const Vector4f& color);

    static Vector3f PinToUnit(const Vector3f& a);
    static Vector4f PinToUnit(const Vector4f& a);

    static Pixel ColorToPixel(const Vector3f& color);
    static Pixel ColorToPixel(const Vector4f& color);

    void OutputPPM(std::string filename) const;
private:
    std::vector<Vector3f> ImageBuffer;
    int Width, Height;
};

#endif /* end of include guard: _RAY_IMAGE_ */
