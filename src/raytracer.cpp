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
#include "Light.h"

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
  Vector3f Color;
public:
  HitData() :
    tMax(100000.0f),
    HitSurface(nullptr),
    Color(0.0f, 0.0f, 0.0f)
  {}
};

class Scene
{
public:
  Scene()
  {
    SceneInit();
  }

  ~Scene()
  {
    std::cout << "Scene destructor called";
  }

  void SceneInit()
  {
    Material* red = Material::CreateRedMat();
    Material* green = Material::CreateGreenMat();
    Material* blue = Material::CreateBlueMat();
    Material* planeMat = new Material(Vector3f(0.2f, 0.2f, 0.2f), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(0.0f, 0.0f, 0.0f), 0);

    Sphere* sphere1 = new Sphere( Vector3f(-4.0f, 0.0f, -7.0f), 1.0f, red);
    Sphere* sphere2 = new Sphere( Vector3f(0.0f, 0.0f, -7.0f), 2.0f, green);
    Sphere* sphere3 = new Sphere( Vector3f(4.0f, 0.0f, -7.0f), 1.0f, blue);
    Sphere* sphere4 = new Sphere( Vector3f(0.0f, 0.0f, -19.0f), 10.0f, blue);

    Plane* plane = new Plane(Vector3f(0.0f, -2.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), planeMat);

    Surfaces.push_back(plane);
    //Surfaces.push_back(sphere4);
    Surfaces.push_back(sphere1);
    Surfaces.push_back(sphere2);
    Surfaces.push_back(sphere3);

    Light* light1 = new Light(Vector3f(-4.0f, 4.0f, -3.0f), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(1.0f, 1.0f, 1.0f));
    Lights.push_back(light1);
  }

  // Call at the end of main to delete all surfaces
  void Destroy()
  {
    for (Surface* s : Surfaces)
    {
      delete s;
    }
  }

  Vector3f Trace(const Ray& ray, float tMin, float tMax)
  {
    HitData Data;

    for (Surface* s : Surfaces)
    {
      if (s->Intersect(ray, Data.t, Data.tMax, Data.tPoint))
      {
        Data.Point      = Data.tPoint;
        Data.t          = Data.tMax;
        Data.HitSurface = s;
        Data.Normal     = Data.HitSurface->GetNormal(Data.Point);
        Data.Color      = Data.HitSurface->GetMaterial()->GetDiffuse();
      }
    }

    for (Light* light : Lights)
    {
      if (Data.HitSurface != nullptr)
      {
        Data.Normal = Data.HitSurface->GetNormal(Data.Point);
        Data.Color = Shade(ray, Data, light);
      }
    }

    return Data.Color;
  }

  Vector3f Shade(const Ray& ray, const HitData& Data, Light* light)
  {
    Vector3f v = -ray.Direction;

    // Calculate vector from Hit point to the light and normalize it
    Vector3f lightdir = light->GetPosition() - Data.Point;
    lightdir.Normalize();

    // Calculate dot product of hit normal and the light vector
    float ndotl = Data.Normal * lightdir;

    if (ndotl < 0.0f)
    {
      std::cout << Data.Normal << ' ' << ndotl << ' ' << std::endl;
    }

    //std::cout << ndotl << ' ';

    // Get diffuse of the hit surface
    Vector3f diff = Data.HitSurface->GetMaterial()->GetDiffuse();
    Vector3f spec = Data.HitSurface->GetMaterial()->GetSpecular();

    // Multiply the diffuse and the clamped cos of the angle
    Vector3f Mdiff = diff * std::max(0.0f, ndotl);
    Vector3f Mspec = spec * std::max(0.0f, ndotl);

    Vector3f Ldiff = Mdiff / M_PI;

    Vector3f half = (lightdir - ray.Direction).Normalized();

    return Mdiff;
  }

public:
  std::vector<Surface*> Surfaces;
  std::vector<Light*> Lights;
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
        Vector3f color = scene->Trace(ray, 0, 10000.0f);
        Pixel p = ColorToPixel(color);
        image.push_back(p);
      }
    }

    OutputPPM(image);
  }

  static Pixel ColorToPixel(const Vector3f& color)
  {
    Vector4f color4(color, 1.0f);
    return ColorToPixel(color4);
  }

  static Pixel ColorToPixel(const Vector4f& color)
  {
    Vector4f normalized = color.Normalized();

    uint8_t uA = (uint8_t) (normalized.w * 255.9999f);
    uint8_t uR = (uint8_t) (normalized.x * uA);
    uint8_t uG = (uint8_t) (normalized.y * uA);
    uint8_t uB = (uint8_t) (normalized.z * uA);

    Pixel pixel = MakePixel(uR, uG, uB, uA);

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
  int ScreenWidth;
  int ScreenHeight;
  float fov, angle;
  float AspectRatio;
};

int main(int argc, char *argv[])
{
  RayTracer* ray = new RayTracer(800, 800);

  Scene scene;

  ray->Render(&scene);

  scene.Destroy();

  exit(EXIT_SUCCESS);
}
