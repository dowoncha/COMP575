#include "RayTracer.h"

RayTracer::RayTracer(Scene const & scene) :
  mScene(scene),
  AspectRatio((float)(ScreenWidth/ScreenHeight)),
  MainCamera(),
  SampleRate(1),
  MaxTraceDepth(3)
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

void RayTracer::BWRender(Image& image) const
{
    LOG(INFO) << "No color render";

    // Buffer to hold color values, reserve size of the window.
    std::vector<glm::vec3> buffer;

    HitData data;
    // Most expensive thing ive ever seen.
    for (int y = 0; y < ScreenHeight; ++y)
    {
        for (int x = 0; x < ScreenWidth; ++x)
        {
            Ray ray = MainCamera.GetRay(x, y);
            glm::vec3 result = BWTrace(ray);
            buffer.push_back(result);
        }
    }

    image.SetBuffer(buffer);
}

void RayTracer::Render(Image& image) const
{
    LOG(INFO) << "Starting rendering to image";

    // Buffer to hold color values, reserve size of the window.
    std::vector<glm::vec3> buffer();

    // Most expensive thing ive ever seen.
    for (int y = 0; y < ScreenHeight; ++y)
    {
        for (int x = 0; x < ScreenWidth; ++x)
        {

            Ray ray = MainCamera.GetRay(x, y);
			      Vector3f result = Sampler(x, y);
            buffer.push_back(result);
        }
    }
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
            glm::vec3 result = Sampler(x, y);
			      Pixel p = Image::ColorToPixel(result);
            buffer.push_back(p);
        }
    }

    return buffer;
}

glm::vec3 RayTracer::Trace(const Ray& ray, int depth) const
{
    // Trace recursion base case
    // depth should stop if bigger than the max trace depth
    if (depth > MaxTraceDepth)
        return glm::vec3(0.0f);        //If max depth has been reached return 0

    // Hit data from the ray trace
    HitData data;
    bool result = mScene.IntersectSurfaces(ray, 10000.0f, data);

    if (data.HitSurface == nullptr)
        return glm::vec3(0.0f);

    // Calculate diffuse and specular here
    glm::vec3 out = Shade(ray, data);

    // If the surface material has a reflection value
    float reflectionCoef = data.HitSurface->Mat().Reflection();

    if (reflectionCoef > 0.0f)
    {
        // Calculate the reflection ray
        glm::vec3 incident = -ray.Direction;
        glm::vec3 Direction = incident - data.Normal * (2.0f * glm::dot( data.Normal, incident ));
        Ray reflectionRay(data.Point, glm::normalize(Direction));

        out += Trace(reflectionRay, depth + 1) * reflectionCoef;
    }

    return out;
}

glm::vec3 RayTracer::Shade(const Ray& ray, const HitData& data) const
{
    /**
     *  For each light in the scene
     *  Calculate the vector from hit point -> light.
     *  Make the shadow ray using the vector
     */
    for (Light* light : mScene.Lights)
    {
        // Trace for shadows here
        Vector3f hitToLight = light->GetPosition() - data.Point;
        Vector3f hitDirection = hitToLight.normalized();
        Ray shadowRay(data.Point + hitDirection, hitDirection);

        // Check for shadows
        if ( !mScene.IntersectSurfaces(shadowRay, len, data.HitSurface))
        {
          Vector3f Diffuse = caculateDiffuse(data, light);
          Vector3f specular = calculateSpecular(ray, data, light);
          result  CalculateDiffuse(data, light) + CalculateSpecular(ray, data, light);
        }
    }

    return result;
}

glm::vec3 RayTracer::CalculateDiffuse(const HitData& data, Light* light) const
{
    // Diffuse light calculations
    // Find the direction vector from hit point to the light, don't forget to normalize
    // Calculate dot product of hit normal and the light vector
    // Multiply the diffuse and the clamped cos of the angle
    glm::vec3 lightdir = glm::normalize(light->GetPosition() - data.Point);
    float ndotl = glm::dot(data.Normal, lightdir);
    glm::vec3 Ldiff = data.HitSurface->Mat().Diffuse() * light->Intensity * std::max(0.0f, ndotl);

    return Ldiff;
}

glm::vec3 RayTracer::CalculateSpecular(const Ray& ray, const HitData& data, Light* light) const
{
    // Specular Light calculations
    // Subtract the ray direction instead of add to reverse direction
    glm::vec3 lightdir = light->GetPosition() - data.Point;
    glm::vec3 half = glm::normalize(lightdir - ray.Direction);
    float ndoth = glm::dot(data.Normal, half);

    // NOTE: I might be cheating here using auto, but not sure if i can get a reference
    // rather than the entire object?
    auto mat = data.HitSurface->Mat();
    glm::vec3 Lspec = mat.Specular() * light->Intensity * std::pow(std::max(0.0f, ndoth), mat.SpecularPower());

    return Lspec;
}

void RayTracer::SetSampleRate(int s)
{
    SampleRate = s;
}

glm::vec3 RayTracer::Sampler(int x, int y) const
{
    switch(SamplingType)
    {
        case UniformSampling:
            return UniformSampler(x, y);
        case RandomSampling:
            return RandomSampler(x, y);
        default:
            LOG(WARNING) << "Sampler not set, default 0 vector returned";
            return glm::vec3(0.0f);
    }
}

glm::vec3 RayTracer::UniformSampler(int x, int y) const
{
    glm::vec3 result;
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

glm::vec3 RayTracer::RandomSampler(int x, int y) const
{
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

    glm::vec3 result;
    int s2 = SampleRate * SampleRate;
    for (int i = 0; i < s2; ++i)
    {
        float offsetx = distribution(generator);
        float offsety = distribution(generator);

        Ray ray = MainCamera.GetRay(x, y, offsetx, offsety);
        result += Trace(ray, 0);
    }

    result = (result / (float) s2);

    return result;
}
