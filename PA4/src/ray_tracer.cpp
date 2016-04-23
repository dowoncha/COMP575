#include "RayTracer.h"

using namespace Eigen;

RayTracer::RayTracer(int* argc, char** argv) :
  scene_(nullptr),
  camera_(new Camera(512, 512)),
  size_(512 * 512),
  frame_buffer_(size_),
  sample_rate_(1),
  max_trace_depth_(2),
  sampler(&NoSampler)
{
   //angle_ = std::tan(M_PI * 0.5f * fov_ / 180.0f);

   // Glut initialization
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(512, 512);
   glutCreateWindow("Ray Tracer");

   // Glew initialization
   glewExperimental = GL_TRUE;
   GLenum err = glewInit();
   if (err != GLEW_OK)
   {
     /* Problem: glewInit failed, something is seriously wrong. */
     fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
     exit(EXIT_FAILURE);
   }
   printf("Status: GLEW %s\n", glewGetString(GLEW_VERSION));

   // glut funcs
   glutIdleFunc(idle);
   glutReshapeFunc(resize);
   glutDisplayFunc(render);
}

RayTracer::~RayTracer()
{}

void RayTracer::initialize(std::unique_ptr<Scene>& scene)
{
  scene_ = std::move(scene);
}

void RayTracer::resize(int width, int height)
{
    LOG(INFO) << "Resize called with width: " << width << ", height: " << height;

    // Resize the camera
    camera->resize(width, height);

    // Compare the new frame buffer size to the old one and allocate as neccessary
    frame_buffer_size_t oldsize = size_;
    size_ = width * height;
    if (size_ < oldsize_)
    {
      frame_buffer.resize(size_);
    }
    else if (size_ > oldsize_)
    {
      frame_buffer.reserve(size)
    }
}

void RayTracer::run()
{
  glutMainLoop();
}

void RayTracer::display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glDrawPixels(camera.screen_width(),
               camera.screen_height(),
               GL_RGBA,
               GL_FLOAT,
               frame_buffer.data());

  glutSwapBuffers();
  glutPostRedisplay();
}

void RayTracer::idle()
{
  render();
}

void RayTracer::Render() const
 {
  LOG(INFO) << "Starting rendering to image";

  // Most expensive thing ive ever seen.
  int index = 0;
  for (int y = 0; y < screen_height_; ++y)
  {
    for (int x = 0; x < screen_width_; ++x, ++index)
    {
      Vector4f result = (*sampling)(x, y);
      frame_buffer_.at(index);
    }
  }
}

Vector4f RayTracer::Trace(const Ray& ray, int depth, int tMax) const
{
    // Trace recursion base case
    // depth should stop if bigger than the max trace depth
    if (depth > max_trace_depth_)
        return Vector4f(0.0f);

    // Hit data from the ray trace
    HitData data;
    bool bHit = scene_.IntersectSurfaces(ray, data);

    // If nothing was hit return black
    if (bHit)
      return Vector4f(0.0f);

    // Local illumination calculation (ambient, specular, diffuse)
    // Additionally calculates shadows
    Vector4f local = LocalShading(ray, data);

    // If the surface material has a reflection value
    // Do another ray trace for reflection surface
    float reflectionCoef = data.hit_surface->material().reflection();
    if (reflectionCoef > 0.0f)
    {
        // Calculate the reflection ray
        Vector3f incident = -ray.direction;
        Vector3f dir = incident - data.normal * (2.0f * data.normal.dot(incident));
        Ray reflectionRay(data.point, dir.normalized());

        Vector3f reflect = Trace(reflectionRay, depth + 1, tMax);

        local = Utility::lerp(local, reflect, reflectionCoef);
    }

    return local;
}

Vector4f RayTracer::LocalShading(const Ray& ray, const HitData& data) const
{
  using namespace std;

  Material* material = materials.at(data.hit_surface->material());

  Vector4f out = material->ambient();

  /**
   *  For each light in the scene
   *  Calculate the vector from hit point -> light.
   *  Make the shadow ray using the vector
   */
  for (Light* light : scene_.lights_)
  {
    // Trace for shadows here
    Vector3f hit_direction = (light->position() - data.hit_point).normalized();
    Ray shadowray(data.hit_point + hit_direction, hit_direction);

    // Check intersection data for shadows
    bool shadowhit = scene_.IntersectSurfaces(shadowRay, len, data.hitSurface);
    if ( shadowhit.hit_surface != nullptr)
    {
      float ndoth = data.normal.dot(half);

      Vector3f Lspec = material->specular() * light->intensity_ * pow(max(0.0f, ndoth), material.specular_power());
      Vector3f normal = data.hit_surface->normal();

      // Get vector from hit point to the light
      Vector3f lightdir = (light->position() - data.hit_point).normalized();

      // Calculate the diffuse lighting color
      float ndotl = normal.dot(lightdir);
      Vector3f Ldiff = material->diffuse() * light->intensity() * max(0.0f, ndotl);

      // Specular Light calculations
      // Subtract the ray direction instead of add to reverse direction
      Vector3f halfdir = (lightdir - ray.direction()).normalized();
      float ndoth = normal.dot(half);

      Vector3f Lspec = mat.specular() * light->intensity_ * pow(max(0.0f, ndoth), material->specular_power());

      out += Ldiff + Lspec;
    }

    return out;
}

void RayTracer::set_sampling_type(PostProcess type)
{
  switch(type)
  {
    case PostProcess::NoSampling:
      sampler = &NoSampling;
      break;
    case PostProcess::UniformSampling:
      sampler = &UniformSampling;
      break;
    case PostProcess::RandomSampling:
      sampler = &RandomSampling;
      break;
    default:
      sampler = &NoSampling;
      break;
  }
}

void RayTracer::set_sample_rate(int sample_rate)
{
    sample_rate_ = sample_rate;
}

Vector4f RayTracer::NoSampling(int x, int y)
{
  Ray ray = camera_->GetRayFromEye(x, y);
  return Trace(ray, max_trace_time_);
}

Vector4f RayTracer::UniformSampling(int x, int y) const
{
    Vector4f result;
    float coef = 1.0f / sample_rate_;
    for (float dy = 0.0f; dy < 1.0f; dy += coef)
    {
        for (float dy = 0.0f; dy < 1.0f; dy += coef)
        {
            Ray ray = camera_->GetRayFromEye(x, y, dx, dy);
            result += Trace(ray, max_trace_time_);
        }
    }
    return result * coef * coef;
}

Vector4f RayTracer::RandomSampling(int x, int y) const
{
  // Random number generator for ray random sampling
  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

  // NOTE: this could be cheaper by avoiding the conversion
  // from int to float, make sampling rate a float.
  Vector4f result;
  float s2 = sample_rate_ * sample_rate_;
  for (int i = 0; i < s2; ++i)
  {
    float dx = distribution(generator);
    float dy = distribution(generator);

    Ray ray = camera_->GetRayFromEye(x, y, dx, dy);
    result += Trace(ray, max_trace_time_);
  }

  // Return the total result divided by the sampling rate^2
  return result / s2;
}
