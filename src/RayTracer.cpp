#include "RayTracer.h"

RayTracer::RayTracer(const Scene& scene, int sWidth, int sHeight) :
  ScreenWidth(sWidth),
  ScreenHeight(sHeight),
  mScene(scene),
  fov(30.0f),
  AspectRatio((float)(ScreenWidth/ScreenHeight)),
  MainCamera()
{
   angle = std::tan(M_PI * 0.5f * fov / 180.0f);
   MainCamera.SetScreenSize(ScreenWidth, ScreenHeight);
}

RayTracer::~RayTracer() { }

void RayTracer::Render(Image& image) const
{
    LOG(INFO) << "Starting rendering";

    // Buffer to hold color values, reserve size of the window.
    std::vector<Vector3f> buffer;

    std::cout << buffer.size();

    HitData data;
    // Most expensive thing ive ever seen.
    for (int y = 0; y < ScreenHeight; ++y)
    {
        for (int x = 0; x < ScreenWidth; ++x)
        {
            Ray ray = MainCamera.GetRay(x, y);
            Vector3f color;
            if (mScene.IntersectSurfaces(ray, 100000.0f, data))
            {
                //buffer.push_back(Vector3f(1.0f));
                buffer.push_back(Shade(ray, data));
            }
            else
                buffer.push_back(Vector3f(0.0f));
        }
    }

    image.SetBuffer(buffer);
}

Vector3f RayTracer::Shade(const Ray& ray, const HitData& data) const
{
    // Initial color is ambient of the hit material
    Vector3f result = data.HitSurface->GetMaterial()->GetAmbient();

    for (Light* light : mScene.Lights)
    {
        // Trace for shadows here
        Vector3f lightDirection = light->GetPosition() - data.Point;
        Ray shadowRay(data.Point, lightDirection.Normalized());

        // If no surface was hit
        if ( !mScene.IntersectSurfaces(shadowRay, lightDirection.Length(), data.HitSurface))
        {
            result += CalculateLight(ray, data, light);
        }
    }

    return result;
}

Vector3f RayTracer::CalculateLight(const Ray& ray, const HitData& data, Light* light) const
{
  // Find the direction vector from hit point to the light.
  Vector3f lightdir = (light->GetPosition() - data.Point).Normalized();

  Material* mat = data.HitSurface->GetMaterial();

  // Diffuse light calculations
  // Calculate dot product of hit normal and the light vector
  // Multiply the diffuse and the clamped cos of the angle
  float ndotl = data.Normal * lightdir;
  Vector3f Ldiff = mat->GetDiffuse() / M_PI * light->Intensity * std::max(0.0f, ndotl);

  // Specular Light calculations
  // Subtract the ray direction instead of add to reverse direction
  Vector3f half = (lightdir - ray.Direction).Normalized();
  float ndoth = data.Normal * half;
  Vector3f Lspec = mat->GetSpecular() * light->Intensity * std::pow(std::max(0.0f, ndoth), mat->GetSpecularPow());

  //Mirror light calculations
  /*
  Vector3f rView = -ray.Direction;
  Ray reflectionRay = data.Normal * 2 * ( data.Normal * rView ) - ray.Direction;
  Vector3f Lmirror(0.0f);
  */

  Vector3f Ltotal = Ldiff + Lspec; //+ Lmirror;

  //std::cout << Ltotal;

  return Vector3f(1.0f);
}

void RayTracer::UniformSampler()
{
}
