#include "ray_tracer.h"

using namespace Eigen;
using namespace raytracer;

RayTracer::RayTracer(int width, int height) :
  camera_(width, height),     // Camera viewport size
  max_trace_depth_(2),        // Max recursion depth of trace function
  bShadows_(true),            // Shadows are enabled
  size_(width * height),      // size of the frame buffer
  frame_buffer_(size_)        // Initially allocate width * height in vector
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

void RayTracer::render(Scene* scene)
{
  int index = 0;
  for (int y = 0; y < camera_->screen_height(); ++y)
  {
    for (int x = 0; x < camera_->screen_width(); ++x, ++index)
    {
      printf("Pixel (%d, %d)\r", x, y);

      Ray ray = camera_.getRay(x, y);
      frame_buffer_.at(index) = gammaEncode(trace(scene, ray));
    }
  }
}

Vector3f RayTracer::trace(Scene* scene, const Ray& ray, int depth)
{
  assert(scene != nullptr);

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

  // Trace recursion base case.
  // it the maximum trace depth has been reached, don't do reflection calculation and
  // return the local shade
  // NOTE: Must come before the reflection calculation
  if (depth == max_trace_depth_) {
    return local;
  }

  // If the surface material has a reflection value
  // Recurisve ray trace for reflection surface
  float reflectionCoef = hit.surface->material()->reflectivity();
  if (reflectionCoef > 0.0f && depth < max_trace_depth_)
  {
      // Calculate the reflection ray
      // recursively trace against the scene
      // lerp the local shading and the reflection shading by the reflection coefficient.
      Vector3f incident = ray.direction();
      Vector3f dir = incident + hit.normal * (-2.0f * hit.normal.dot(incident));
      Ray reflectionRay(hit.point, dir);

      // Reflected ray traced color
      Vector3f reflect = trace(scene, reflectionRay, depth + 1);

      return Utility::lerp(local, reflect, reflectionCoef);
  }

  return local;
}

Vector3f RayTracer::localShading(Scene* scene, const Ray& ray, const HitData& hit) const
{
  using namespace std;

  assert(scene != nullptr);
  assert(hit.surface != nullptr);

  // Get raw pointer to material by its name.
  Material* material = materials_[hit.surface->material()].get();

  // Every color has an ambient.
  Vector3f out = material->ambient();

  /**
   *  For each light in the scene
   *  Calculate the shadow ray (vvector) from hit point -> light.
   */
  for (const light_t& light : scene->lights_)
  {
    // Trace for shadows here
    Vector3f hit_to_light = light->position() - hit.point;
    Vector3f shadow_dir = hit_to_light.normalized();

    // If shadows are on then check intersections using shadowray.
    // If it hits an object move onto next light.
    if (bShadows_)
    {
      Ray shadowray(hit.point + hit.normal * 1e-3, shadow_dir);
      if (scene->intersectSurfaces(shadowRay)) continue;
    }

    // If shadows are off skip intersecting
    // Local lighting calculation
    // Calculate the diffuse lighting color
    float ndotl = hit.normal.dot(shadow_dir);
    Vector3f Ldiff = material->diffuse() * light->intensity() * max(0.0f, ndotl);

    // Specular Light calculations
    // Subtract the ray direction instead of add to reverse direction
    Vector3f hit_to_eye = (ray.position() - hit.point).normalized();
    Vector3f half = (shadow_dir + hit_to_eye).normalized();
    float ndoth = hit.normal.dot(half);
    Vector3f Lspec = material->specular() * light->intensity() * pow(max(0.0f, ndoth), material->specular_power());

    out += Ldiff + Lspec;
  }

  //printf("Local shading end\n");

  return out;
}

Vector3f RayTracer::gammaEncode(const Vector3f& color)
{
  float gamma = 1.0f / 2.2f;

  Vector3f out;

  out(0) = std::pow(color(0), gamma);
  out(1) = std::pow(color(1), gamma);
  out(2) = std::pow(color(2), gamma);

  out(0) = Utility::PinToUnit(out(0));
  out(1) = Utility::PinToUnit(out(1));
  out(2) = Utility::PinToUnit(out(2));

  return out;
}
