#include "RayTracer.h"

RayTracer::RayTracer(const Scene& scene, int sWidth, int sHeight) :
  ScreenWidth(sWidth),
  ScreenHeight(sHeight),
  mScene(scene),
  fov(30.0f),
  AspectRatio((float)(ScreenWidth/ScreenHeight)),
  MainCamera(),
  SampleRate(1)
{
   angle = std::tan(M_PI * 0.5f * fov / 180.0f);
   MainCamera.SetScreenSize(ScreenWidth, ScreenHeight);
   SamplingType = PostProcess::UniformSampling;
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
            Vector3f result = Sampler(x, y);
            buffer.push_back(result);
        }
    }

    image.SetBuffer(buffer);
}

Vector3f RayTracer::Trace(const Ray& ray, int depth) const
{
    static int dMax = 3;
    if (depth > dMax) return Vector3f(0.0f);        //If max depth has been reached return 0

    HitData data;
    bool result = mScene.IntersectSurfaces(ray, 1000.0f, data);
    if (data.HitSurface == nullptr) return Vector3f(0.0f);

    Vector3f out = Shade(ray, data);

    float reflec = data.HitSurface->GetMaterial().GetReflection();

    if (reflec > 0.0f)
    {
        Vector3f incident = -ray.Direction;
        Vector3f Direction = incident - data.Normal * 2 * ( data.Normal * incident );
        Ray reflectionRay(data.Point, Direction.Normalized());

        //out += Trace(reflectionRay, depth + 1) * reflec;
    }

    return out;
}

Vector3f RayTracer::Shade(const Ray& ray, const HitData& data) const
{
    // Initial color is ambient of the hit material
    Vector3f result = data.HitSurface->GetMaterial().GetAmbient();

    for (Light* light : mScene.Lights)
    {
        // Trace for shadows here
        Vector3f shadowDir = light->GetPosition() - data.Point;
        float len = shadowDir.Length();
        shadowDir.Normalize();
        Ray shadowRay(data.Point + shadowDir, shadowDir);

        // Check for shadows
        if ( !mScene.IntersectSurfaces(shadowRay, len, data.HitSurface))
        {
            result += CalculateDiffuse(data, light) + CalculateSpecular(ray, data, light);
        }
    }

    return result;
}

Vector3f RayTracer::CalculateDiffuse(const HitData& data, Light* light) const
{
    // Diffuse light calculations
    // Find the direction vector from hit point to the light, don't forget to normalize
    // Calculate dot product of hit normal and the light vector
    // Multiply the diffuse and the clamped cos of the angle
    Vector3f lightdir = (light->GetPosition() - data.Point).Normalized();
    float ndotl = data.Normal * lightdir;
    Vector3f Ldiff = data.HitSurface->GetMaterial().GetDiffuse() * light->Intensity * std::max(0.0f, ndotl);

    return Ldiff;
}

Vector3f RayTracer::CalculateSpecular(const Ray& ray, const HitData& data, Light* light) const
{
    // Specular Light calculations
    // Subtract the ray direction instead of add to reverse direction
    Vector3f lightdir = light->GetPosition() - data.Point;
    Vector3f half = (lightdir - ray.Direction).Normalized();
    float ndoth = data.Normal * half;
    Material mat = data.HitSurface->GetMaterial();

    Vector3f Lspec = mat.GetSpecular() * light->Intensity * std::pow(std::max(0.0f, ndoth), mat.GetSpecularPow());

    return Lspec;
}

// Calculate both spec and diffuse in one function
Vector3f RayTracer::CalculateLight(const Ray& ray, const HitData& data, Light* light) const
{
    Vector3f lightdir = (light->GetPosition() - data.Point).Normalized();
    Material mat = data.HitSurface->GetMaterial();

    float ndotl = data.Normal * lightdir;
    Vector3f Ldiff = mat.GetDiffuse() * light->Intensity * std::max(0.0f, ndotl);

    Vector3f half = (lightdir - ray.Direction).Normalized();
    float ndoth = data.Normal * half;
    Vector3f Lspec = mat.GetSpecular() * light->Intensity * std::pow(std::max(0.0f, ndoth), mat.GetSpecularPow());

    Vector3f Ltotal = Ldiff + Lspec;

    return Ltotal;
}

void RayTracer::SetSampleRate(int s)
{
    SampleRate = s;
}

Vector3f RayTracer::Sampler(int x, int y) const
{
    switch(SamplingType)
    {
        case UniformSampling:
            return UniformSampler(x, y);
        case RandomSampling:
            return RandomSampler(x, y);
        default:
            return Vector3f(0.0f);
    }
}

Vector3f RayTracer::UniformSampler(int x, int y) const
{
    Vector3f result;
    float coef = 1.0f / SampleRate;
    for (float offsetx = 0.0f; offsetx < 1.0f; offsetx += coef)
    {
        for (float offsety = 0.0f; offsety < 1.0f; offsety += coef)
        {
            Ray ray = MainCamera.GetRay(x, y, offsetx, offsety);
            result += Trace(ray, 0);
        }
    }
    return result * coef * coef;
}

Vector3f RayTracer::RandomSampler(int x, int y) const
{
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    Vector3f result;
    int s2 = SampleRate * SampleRate;
    for (int i = 0; i < s2; ++i)
    {
        float offsetx = distribution(generator);
        float offsety = distribution(generator);

        Ray ray = MainCamera.GetRay(x, y, offsetx, offsety);
        result += Trace(ray, 0);
    }

    result = result / s2;

    return result;
}
