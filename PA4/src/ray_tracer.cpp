#include "ray_tracer.h"

using namespace Eigen;
using namespace raytracer;

RayTracer::RayTracer() :
  camera_(new Camera(512, 512)),
  size_(512 * 512),
  max_trace_depth_(2)
{
}

RayTracer::~RayTracer()
{}

void RayTracer::bwrender(Scene* scene)
{
  std::vector<Vector4f> buffer;

  int index = 0;
  for (int y = 0; y < camera_->screen_height(); ++y)
  {
    for (int x = 0; x < camera_->screen_width(); ++x, ++index)
    {
      if (index > size_)
      {
        fprintf(stderr, "x and y are past the buffer size, x: %d, y: %d, size: %lu\n", x, y, size_);
        return;
      }

      Ray ray = camera_->GetRayFromEye(x, y);
      HitData hit;
      if (!scene->intersect_surfaces(ray, hit)) buffer.push_back(Vector4f(0.0f, 0.0f, 0.0f, 1.0f));
      else buffer.push_back(Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
    }
  }

  frame_buffer_ = buffer;
}

void RayTracer::render(Scene* scene)
{
  printf("Rendering buffer size: %lu\n", frame_buffer_.size());

  std::vector<Vector4f> buffer;

  int index = 0;
  for (int y = 0; y < camera_->screen_height(); ++y)
  {
    for (int x = 0; x < camera_->screen_width(); ++x, ++index)
    {
      if (index > size_)
      {
        fprintf(stderr, "x and y are past the buffer size, x: %d, y: %d, size: %lu\n", x, y, size_);
        return;
      }

      Ray ray = camera_->GetRayFromEye(x, y);
      buffer.push_back(trace(scene, ray, 0));
    }
  }

  frame_buffer_ = buffer;

  printf("Fb size: %lu, size_: %lu\n", frame_buffer_.size(), size_);
  assert(buffer.size() == size_);
}

Vector4f RayTracer::trace(Scene* scene, const Ray& ray, int depth = 0)
{
  assert(scene != nullptr);

  // Trace recursion base case
  // depth should stop if bigger than the max trace depth
  if (depth > max_trace_depth_)
    return Vector4f(0.0f, 0.0f, 0.0f, 1.0f);

  // Intersect ray against all surfaces and return hit data of closest hit
  // If nothing was hit return black, base case
  HitData hit;
  if (!scene->intersect_surfaces(ray, hit))
  {
    //printf("Recursion depth: %d\n", depth);
    return Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
  }

  // Calculate ambient, diffuse, and specular, and shadows
  Vector4f local = local_shading(scene, ray, hit);

  // If the surface material has a reflection value
  // Recurisve ray trace for reflection surface
  float reflectionCoef = hit.surface->material()->reflectivity();
  if (reflectionCoef > 0.0f && depth <= max_trace_depth_)
  {
      // Calculate the reflection ray
      Vector3f incident = ray.direction();
      Vector3f dir = incident - hit.normal * (2.0f * hit.normal.dot(incident));
      Ray reflectionRay(hit.point, dir.normalized());

      Vector4f reflect = trace(scene, reflectionRay, depth + 1);
      // Lerp
      local = Utility::lerp(local, reflect, reflectionCoef);
  }

  //printf("Finished tracing\n");

  return local;
}

Vector4f RayTracer::local_shading(Scene* scene, const Ray& ray, const HitData& hit) const
{
  //printf("Local shading\n");

  using namespace std;

  Material* material = hit.surface->material();

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
    Ray shadowray(hit.point + shadow_dir, shadow_dir);

    if (!scene->intersect_surfaces(shadowray, hit.surface))
    {
      // Calculate the diffuse lighting color
      float ndotl = hit.normal.dot(shadow_dir);
      Vector4f Ldiff = material->diffuse() * light->intensity() * max(0.0f, ndotl);

      // Specular Light calculations
      // Subtract the ray direction instead of add to reverse direction
      Vector3f half = (hit_to_light - ray.direction()).normalized();
      float ndoth = hit.normal.dot(half);
      Vector4f Lspec = material->specular() * light->intensity() * pow(max(0.0f, ndoth), material->specular_power());

      out += Ldiff + Lspec;
    }
  }

  //printf("Local shading end\n");

  return out;
}
