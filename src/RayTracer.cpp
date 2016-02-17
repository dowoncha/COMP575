#include "RayTracer.h"

RayTracer::RayTracer(const Scene& scene, int sWidth, int sHeight) :
  ScreenWidth(sWidth),
  ScreenHeight(sHeight),
  mScene(scene),
  fov(30.0f),
  AspectRatio((float)(ScreenWidth/ScreenHeight)),
  MainCamera(),
  SampleRate(1),
  MaxTraceDepth(2)
{
   angle = std::tan(M_PI * 0.5f * fov / 180.0f);
   MainCamera.SetScreenSize(ScreenWidth, ScreenHeight);
   SamplingType = PostProcess::UniformSampling;
}

RayTracer::~RayTracer() { }

void RayTracer::Resize(int w, int h)
{
    LOG(INFO) << "Resize called with width: " << w << ", height: " << h;
    ScreenWidth = w;
    ScreenHeight = h;
    MainCamera.SetScreenSize(w, h);
}

void RayTracer::Render(Image& image) const
{
    LOG(INFO) << "Starting rendering to image";

    // Buffer to hold color values, reserve size of the window.
    std::vector<Vector3f> buffer;

    // Most expensive thing ive ever seen.
    for (int y = 0; y < ScreenHeight; ++y)
    {
        for (int x = 0; x < ScreenWidth; ++x)
        {
            Ray ray = MainCamera.GetRay(x, y);
            Vector3f result = Trace(ray, 0); //Sampler(x, y);
            buffer.push_back(result);
        }
    }

    image.SetBuffer(buffer);
}

std::vector<Pixel> RayTracer::Render() const
{
    LOG(INFO) << "Ray tracer render function for GL";

    // generate buffer for window size
    std::vector<Pixel> buffer;

    for (int y = 0; y < ScreenHeight; ++y)
    {
        for (int x = 0; x < ScreenWidth; ++x )
        {
            Vector3f result = Sampler(x, y);
            Pixel p = MakePixel(result.x, result.y, result.z, 255);
            buffer.push_back(p);
        }
    }

    return buffer;
}

Vector3f RayTracer::Trace(const Ray& ray, int depth) const
{
    if (depth > MaxTraceDepth)
        return Vector3f(0.0f);        //If max depth has been reached return 0

    HitData data;
    bool result = mScene.IntersectSurfaces(ray, 10000.0f, data);

    if (data.HitSurface == nullptr)
        return Vector3f(0.0f);

    // Calculate diffuse and specular here
    Vector3f out = data.HitSurface->GetMaterial().GetDiffuse(); //Shade(ray, data);

    // If the surface material has a reflection value
    float reflectionCoef = data.HitSurface->GetMaterial().GetReflection();

    if (reflectionCoef > 0.0f)
    {
        // Calculate the reflection ray
        Vector3f incident = -ray.Direction;
        Vector3f Direction = incident - data.Normal * 2 * ( data.Normal * incident );
        Ray reflectionRay(data.Point, Direction.Normalized());

        //out += Trace(reflectionRay, depth + 1) * reflectionCoef;
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

    // I might be cheating here using auto, but not sure if i can get a reference
    // rather than the entire object.
    auto mat = data.HitSurface->GetMaterial();

    Vector3f Lspec = mat.GetSpecular() * light->Intensity * std::pow(std::max(0.0f, ndoth), mat.GetSpecularPow());

    return Lspec;
}

// Calculate both spec and diffuse in one function
Vector3f RayTracer::CalculateLight(const Ray& ray, const HitData& data, Light* light) const
{
    Vector3f lightdir = (light->GetPosition() - data.Point).Normalized();
    auto mat = data.HitSurface->GetMaterial();

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
            LOG(WARNING) << "Sampler not set, default 0 vector returned";
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
