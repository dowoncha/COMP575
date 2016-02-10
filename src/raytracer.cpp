/*********************************************************************************
 *
 * filename  : RayTracer.cpp
 * author    : Do Won Cha
 * content   : This is a ray tracer to draw some spheres.
 *
 *********************************************************************************/

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <vector>
#include <list>
#include <iostream>
#include <cassert>

#include "Primitives.h"
#include "Camera.h"
#include "Ray.h"
#include "Surfaces.h"

#ifdef _WIN32
#define M_PI 3.14159265359
#elif __unix__ || __APPLE__
#endif

// Typedef a pixel as a unsigned 32 bit integer
using Pixel = uint32_t;
static Pixel MakePixel(uint8_t r, uint8_t g, uint8_t b, uint8_t alpha)
{
  Pixel p = (uint32_t)((r << 24) | (g << 16) | (b << 8) | alpha);
  return p;
}

static uint8_t GetPixelR(Pixel p) { return (p >> 24) & 0xff; }
static uint8_t GetPixelG(Pixel p) { return (p >> 16) & 0xff; }
static uint8_t GetPixelB(Pixel p) { return (p >> 8 ) & 0xff; }
static uint8_t GetPixelA(Pixel p) { return p & 0xff;         }

class HitData
{
public:
  Vector3f Point, tPoint;
  Vector3f Normal;
  float t, tMax;
  Surface* HitSurface;
  Vector4f Color;
public:
  HitData() :
    tMax(100000.0f),
    HitSurface(nullptr),
    Color(0.0f)
  {}
};

class Scene
{
public:
  Scene()
  {
    SceneInit();
  }

  void SceneInit()
  {
    Sphere* sphere1 = new Sphere( Vector3f(-4.0f, 0.0f, -7.0f), 1.0f);
    Sphere* sphere2 = new Sphere( Vector3f(0.0f, 0.0f, -7.0f), 2.0f);
    Sphere* sphere3 = new Sphere( Vector3f(4.0f, 0.0f, -7.0f), 1.0f);

    Surfaces.push_back(sphere1);
    Surfaces.push_back(sphere2);
    Surfaces.push_back(sphere3);
  }

  void Destroy()
  {
    for (auto it = Surfaces.begin(); it != Surfaces.end(); ++it)
    {
      delete *it;
    }
  }

  void AddSurface(Surface* s)
  {
    Surfaces.push_back(s);
  }

  Vector4f Trace(const Ray& ray, float tMin, float tMax)
  {
    HitData Data;

    //for (auto it = Surfaces.begin(); it != Surfaces.end(); ++it)
    for (Surface* s : Surfaces)
    {
      //std::cout << "Tracing surface";
      if (s->Intersect(ray, Data.t, Data.tMax, Data.tPoint))
      {
        std::cout << "Intersected!\n";
        Data.Point      = Data.tPoint;
        Data.t          = Data.tMax;
        Data.HitSurface = s;
        Data.Color = Vector4f(1.0f);
      }
    }

    return Data.Color;
  }

  void Shade(const Ray& ray, const Vector3f& Point, const Vector3f& Normal)
  {
  }

public:
  std::vector<Surface*> Surfaces;
};

class RayTracer
{
public:
  RayTracer(int sWidth, int sHeight) :
    ScreenWidth(sWidth),
    ScreenHeight(sHeight),
    fov(30.0f),
    AspectRatio((float)(ScreenWidth/ScreenHeight)),
    MainCamera()
  {
     angle = std::tan(M_PI * 0.5f * fov / 180.0f);
	   MainCamera.SetScreenSize(ScreenWidth, ScreenHeight);
  }

  ~RayTracer() { }

  void Render(Scene* scene)
  {
    std::vector<Pixel> image;

    for (int y = 0; y < ScreenHeight; ++y)
    {
      for (int x = 0; x < ScreenWidth; ++x)
      {
        Ray ray = MainCamera.GetRay( x, y);
        Vector4f color = scene->Trace(ray, 0, 10000.0f);
        //std::cout << color << ' ';
        Pixel p = ColorToPixel(color);
        image.push_back(p);
      }
    }

    OutputPPM(image);
  }

  static Pixel ColorToPixel(const Vector4f& color)
  {
    Vector4f normalized = color.Normalized();

    uint8_t uA = (uint8_t) (normalized.w * 255.9999f);
    uint8_t uR = (uint8_t) (normalized.x * uA);
    uint8_t uG = (uint8_t) (normalized.y * uA);
    uint8_t uB = (uint8_t) (normalized.z * uA);

    Pixel pixel = (uA << 24) | (uR << 16) | (uG << 8) | uB;

    return pixel;
  }

  std::vector<Pixel> ConvertColorToPixel(const std::vector<Vector4f>& colors)
  {
    std::vector<Pixel> p;

    for (Vector4f color: colors)
    {
      p.push_back(ColorToPixel(color));
    }

    return p;
  }

  void OutputPPM(const std::vector<Pixel>& image)
  {
  	// Open stream and write ppm headers, color bit's set to 255 per channel
  	std::ofstream ofs("./test.ppm", std::ios::out | std::ios::binary);
  	ofs << "P6\n" << ScreenWidth << " " << ScreenHeight << "\n255\n";

  	for (Pixel p : image)
  	{
  		ofs << GetPixelR(p) << GetPixelG(p) << GetPixelB(p);
  	}
  }

private:
  Camera MainCamera;
  int ScreenWidth = 800;
  int ScreenHeight = 600;
  float fov, angle;
  float AspectRatio;
};

int main(int argc, char *argv[])
{
  RayTracer* ray = new RayTracer(512, 512);

  Scene scene;

  std::cout << scene.Surfaces.size();

  ray->Render(&scene);

  scene.Destroy();

  exit(EXIT_SUCCESS);
}
