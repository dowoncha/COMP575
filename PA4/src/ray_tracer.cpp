#include "ray_tracer.h"

using namespace Eigen;
using namespace raytracer;

RayTracer::RayTracer(int* argc, char** argv) :
  scene_(nullptr),
  camera_(new Camera(512, 512)),
  size_(512 * 512),
  frame_buffer_(size_),
  sample_rate_(1),
  max_trace_depth_(2),
  sampler(&RayTracer::NoSampling)
{
   //angle_ = std::tan(M_PI * 0.5f * fov_ / 180.0f);

   // Glut initialization
   glutInit(argc, argv);
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
   glutIdleFunc(Idle);
   glutReshapeFunc(resize);
   glutDisplayFunc(Display);
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
    camera_->resize(width, height);

    // Compare the new frame buffer size to the old one and allocate as neccessary
    frame_buffer_size_t oldsize = size_;
    size_ = width * height;
    if (size_ < oldsize)
    {
      frame_buffer_.resize(size_);
    }
    else if (size_ > oldsize)
    {
      frame_buffer_.reserve(size_);
    }
}

void RayTracer::run()
{
  glutMainLoop();
}

void RayTracer::Display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glDrawPixels(camera_->screen_width(),
               camera_->screen_height(),
               GL_RGBA,
               GL_FLOAT,
               frame_buffer_.data());

  glutSwapBuffers();
  glutPostRedisplay();
}

void RayTracer::Idle()
{
  Render();
}

void RayTracer::Render()
 {
  LOG(INFO) << "Starting rendering to image";

  // Most expensive thing ive ever seen.
  int index = 0;
  for (int y = 0; y < camera_->screen_height(); ++y)
  {
    for (int x = 0; x < camera_->screen_width(); ++x, ++index)
    {
      Vector4f result = ((*this).*(sampler))(x, y);
      frame_buffer_.at(index);
    }
  }
}

Vector4f RayTracer::Trace(const Ray& ray, int depth) const
{
    // Trace recursion base case
    // depth should stop if bigger than the max trace depth
    if (depth > max_trace_depth_)
        return Vector4f(0.0f);

    // Hit data from the ray trace
    HitData data;
    bool bHit = scene_->IntersectSurfaces(ray, data);

    // If nothing was hit return black
    if (bHit)
      return Vector4f(0.0f);

    // Local illumination calculation (ambient, specular, diffuse)
    // Additionally calculates shadows
    Vector4f local = LocalShading(ray, data);

    // If the surface material has a reflection value
    // Do another ray trace for reflection surface
    float reflectionCoef = scene_->materials_.at(data.hit_surface->material())->reflectivity();
    if (reflectionCoef > 0.0f)
    {
        // Calculate the reflection ray
        Vector3f incident = -ray.direction();
        Vector3f dir = incident - data.normal * (2.0f * data.normal.dot(incident));
        Ray reflectionRay(data.hit_point, dir.normalized());

        Vector4f reflect = Trace(reflectionRay, depth + 1);

        local = Utility::lerp(local, reflect, reflectionCoef);
    }

    return local;
}

Vector4f RayTracer::LocalShading(const Ray& ray, const HitData& data) const
{
  using namespace std;

  Material* material = scene_->materials_.at(data.hit_surface->material()).get();

  Vector4f out = material->ambient();

  /**
   *  For each light in the scene
   *  Calculate the vector from hit point -> light.
   *  Make the shadow ray using the vector
   */
  for (const std::unique_ptr<Light>& uLight : scene_->lights_)
  {
    Light* light = uLight.get();

    // Trace for shadows here
    Vector3f hitToLight = light->position() - data.hit_point;
    Ray shadowray(data.hit_point + hitToLight.normalized(), hitToLight);
    HitData shadowhit;

    // Check intersection data for shadows, ignore data.hit_surface
    bool bShadow = scene_->IntersectSurfaces(shadowray, shadowhit, data.hit_surface);
    if ( shadowhit.hit_surface != nullptr)
    {
      // Calculate the diffuse lighting color
      float ndotl = data.normal.dot(hitToLight.normalized());
      Vector4f Ldiff = material->diffuse() * light->intensity_ * max(0.0f, ndotl);

      // Specular Light calculations
      // Subtract the ray direction instead of add to reverse direction
      Vector3f halfdir = (hitToLight.normalized() - ray.direction()).normalized();
      float ndoth = data.normal.dot(halfdir);
      Vector4f Lspec = material->specular() * light->intensity_ * pow(max(0.0f, ndoth), material->specular_power());

      out += Ldiff + Lspec;
    }
  }

  return out;
}

void RayTracer::set_sampling_type(PostProcess type)
{
  switch(type)
  {
    case PostProcess::NoSampling:
      sampler = &RayTracer::NoSampling;
      break;
    case PostProcess::UniformSampling:
      sampler = &RayTracer::UniformSampling;
      break;
    case PostProcess::RandomSampling:
      sampler = &RayTracer::RandomSampling;
      break;
    default:
      sampler = &RayTracer::NoSampling;
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

Vector4f RayTracer::UniformSampling(int x, int y)
{
    Vector4f result;
    float coef = 1.0f / sample_rate_;
    for (float dy = 0.0f; dy < 1.0f; dy += coef)
    {
        for (float dx = 0.0f; dx < 1.0f; dx += coef)
        {
            Ray ray = camera_->GetRayFromEye(x, y, dx, dy);
            result += Trace(ray, max_trace_time_);
        }
    }
    return result * coef * coef;
}

Vector4f RayTracer::RandomSampling(int x, int y)
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
