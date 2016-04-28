#include "ray_tracer.h"

using namespace Eigen;
using namespace raytracer;

RayTracer::RayTracer() :
  camera_(new Camera()),
  size_(camera_->screen_width() * camera_->screen_height()),
  max_trace_depth_(2),
  bShadows_(true)
{
}

RayTracer::RayTracer(int width, int height) :
  camera_(new Camera(width, height)),
  size_(width * height),
  max_trace_depth_(2),
  bShadows_(true),
  frame_buffer_(size_)
{
}

RayTracer::~RayTracer()
{}

void RayTracer::setMaxTraceDepth(int new_depth)
{
  max_trace_depth_ = new_depth;
}

void RayTracer::enableShadows()
{
  bShadows = true;
}

void RayTracer::disableShadows()
{
  bShadows = false;
}

void RayTracer::bwrender(Scene* scene)
{
  int index = 0;
  for (int y = 0; y < camera_->screen_height(); ++y)
  {
    for (int x = 0; x < camera_->screen_width(); ++x, ++index)
    {
      Ray ray = camera_->getRay(x, y);
      HitData hit;
      if (!scene->intersect_surfaces(ray, hit))
        frame_buffer_.at(index) = Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
      else
      {
        printf("Pixel (%d, %d)\r", x, y);
        frame_buffer_.at(i) = Vector4f(1.0f, 1.0f, 1.0f, 1.0f);
      }
    }
  }
}

/**
 *  Ray trace the input scene and fill the frame buffer with traced result
 *  @param scene weak pointer to the scene to render
 */
void RayTracer::render(Scene* scene)
{
  int index = 0;
  for (int y = 0; y < camera_->screen_height(); ++y)
  {
    for (int x = 0; x < camera_->screen_width(); ++x, ++index)
    {
      printf("Pixel (%d, %d)\r", x, y);

      Ray ray = camera_->getRay(x, y);
      frame_buffer_.at(index) = gammaEncode(trace(scene, ray, 0));
    }
  }
}

Vector4f RayTracer::trace(Scene* scene, const Ray& ray, int depth = 0)
{
  assert(scene != nullptr);

  // Trace recursion base case
  // depth should stop if equal than the max trace depth
  if (depth == max_trace_depth_)
    return Vector4f(0.0f, 0.0f, 0.0f, 1.0f);

  // Intersect ray against all surfaces and return hit data of closest hit
  // If nothing was hit return black, base case
  HitData hit;
  if (!scene->intersect_surfaces(ray, hit))
  {
    //printf("Recursion depth: %d\n", depth);
    return Vector4f(0.0f, 0.0f, 0.0f, 1.0f);
  }

  // Calculate ambient, diffuse, and specular, and shadows
  Vector4f local = local_shading(scene, ray, hit);

  // If the surface material has a reflection value
  // Recurisve ray trace for reflection surface
  float reflectionCoef = hit.surface->material()->reflectivity();
  if (reflectionCoef > 0.0f && depth < max_trace_depth_)
  {
      // Calculate the reflection ray
      Vector3f incident = ray.direction();
      Vector3f dir = incident + hit.normal * (-2.0f * hit.normal.dot(incident));
      Ray reflectionRay(hit.point, dir);

      Vector4f reflect = trace(scene, reflectionRay, depth + 1);

      // Lerp
      local = Utility::lerp(local, reflect, reflectionCoef);
  }

  //printf("Finished tracing\n");

  return local;
}

Vector4f RayTracer::localShading(Scene* scene, const Ray& ray, const HitData& hit) const
{
  assert(scene != nullptr);
  assert(hit.surface != nullptr);
    //printf("Local shading\n");

  using namespace std;

  Material* material = hit.surface->material();

  assert(material != nullptr);

  Vector4f out = material->ambient();

  /**
   *  For each light in the scene
   *  Calculate the shadow ray (vvector) from hit point -> light.
   */
  for (const light_t& light : scene->lights_)
  {
    // Trace for shadows here
    Vector3f hit_to_light = light->position() - hit.point;
    Vector3f shadow_dir = hit_to_light.normalized();
    Ray shadowray(hit.point + hit.normal * 1e-2, shadow_dir);

    // Local lighting calculation
    if (!scene->intersect_surfaces(shadowray))
    {
      // Calculate the diffuse lighting color
      float ndotl = hit.normal.dot(shadow_dir);
      Vector4f Ldiff = material->diffuse() * light->intensity() * max(0.0f, ndotl);

      // Specular Light calculations
      // Subtract the ray direction instead of add to reverse direction
      Vector3f hit_to_eye = (ray.position() - hit.point).normalized();
      Vector3f half = (shadow_dir + hit_to_eye).normalized();
      float ndoth = hit.normal.dot(half);
      Vector4f Lspec = material->specular() * light->intensity() * pow(max(0.0f, ndoth), material->specular_power());

      out += Ldiff + Lspec;
    }
  }

  //printf("Local shading end\n");

  return out;
}

Vector4f RayTracer::gammaEncode(const Vector4f& color)
{
  float gamma = 1.0f / 2.2f;

  Vector4f out;

  out(0) = std::pow(color(0), gamma);
  out(1) = std::pow(color(1), gamma);
  out(2) = std::pow(color(2), gamma);

  out(0) = Utility::PinToUnit(out(0));
  out(1) = Utility::PinToUnit(out(1));
  out(2) = Utility::PinToUnit(out(2));

  out(3) = 1.0f;

  return out;
}
