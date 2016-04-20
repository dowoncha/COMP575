#include "RayTracer.h"

RayTracer::RayTracer(const Scene& scene) :
  mScene(scene),
  AspectRatio((float)(ScreenWidth/ScreenHeight)),
  MainCamera(512, 512),
  frameBufferSize(512 * 512),
  SampleRate(1),
  MaxTraceDepth(2),
  SamplingType(PostProcess::UniformSampling)
{
   angle = std::tan(M_PI * 0.5f * fov / 180.0f);

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
}

RayTracer::~RayTracer() { }

void RayTracer::init()
{
  // Setup depth buffer, shading, and culling
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  //glDisable(GL_CULL_FACE);
  glDepthFunc(GL_LESS);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_RESCALE_NORMAL);

  glutReshapeFunc(resize);
  glutDisplayFunc(render);
}

void RayTracer::resize(int w, int h)
{
    LOG(INFO) << "Resize called with width: " << w << ", height: " << h;
    camera.resize(w, h);
    frameBufferSize = w * h;
    frameBuffer.resize(frameBufferSize);
}

void RayTracer::run()
{
  glutMainLoop();
}

void RayTracer::render() const
{
  lClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  start_timing();

  LOG(INFO) << "Starting rendering to image";

  // Most expensive thing ive ever seen.
  for (int y = 0; y < ScreenHeight; ++y)
  {
      for (int x = 0; x < ScreenWidth; ++x)
      {
          Ray ray = MainCamera.GetRay(x, y);
          Vector3f result = Sampler(x, y);
          buffer.at(result);
      }
  }

  // glDrawPixels here

  float timeElapsed = stop_timing();
  gTotalFrames++;
  gTotalTimeElapsed += timeElapsed;
  float fps = gTotalFrames / gTotalTimeElapsed;
  char string[1024] = {0};
  sprintf(string, "Ray tracer FPS: %0.2f", fps);
  glutSetWindowTitle(string);

  glutPostRedisplay();
  glutSwapBuffers();
}

Vector3f RayTracer::trace(const Ray& ray, int depth, int tMax) const
{
    // Trace recursion base case
    // depth should stop if bigger than the max trace depth
    if (depth > MaxTraceDepth)
        return Vector4f(0.0f);        //If max depth has been reached return 0

    // Hit data from the ray trace, don't ignore any surfaces
    HitData data = mScene.IntersectSurfaces(ray, tMax);

    // If nothing was hit return color black
    if (data.HitSurface == nullptr)
        return Vector4f(0.0f);

    // Local illumination calculation (ambient, specular, diffuse)
    // Calculates shadowing as well
    Vector3f local = shade(ray, data);

    // If the surface material has a reflection value
    // Do another ray trace for reflection surface
    float reflectionCoef = data.HitSurface->Mat().Reflection();
    if (reflectionCoef > 0.0f)
    {
        // Calculate the reflection ray
        Vector3f incident = -ray.direction;
        Vector3f dir = incident - data.normal * (2.0f * data.normal.dot(incident));
        Ray reflectionRay(data.point, dir.normalized());

        Vector3f reflect = trace(reflectionRay, depth + 1, tMax);

        local = Utility::lerp(local, reflect, reflectionCoef);
    }

    return local;
}

Vector3f RayTracer::shade(const Ray& ray, const HitData& data) const
{
  Vector4f out = data.hitSurface->material().ambient();

  /**
   *  For each light in the scene
   *  Calculate the vector from hit point -> light.
   *  Make the shadow ray using the vector
   */
  for (Light* light : mScene.Lights)
  {
    // Trace for shadows here
    Vector3f hitDir = (light->GetPosition() - data.Point).normalized();
    Ray shadowRay(data.Point + hitDir, hitDir);

    // Check intersection data for shadows
    HitData hit = mScene.intersectSurface(shadowRay, len, data.hitSurface);
    if ( hit.hitSurface != nullptr)
    {
      float ndoth = data.normal.dot(half);

      const Material& mat = data.HitSurface->material();
      Vector3f Lspec = mat.specular() * light->intensity * std::pow(std::max(0.0f, ndoth), mat.specPower());
      Vector3f normal = data.hitSurface->getNormal();

      Vector3f lightdir = (light->position() - data.Point.normalized();

      // Calculate the diffuse lighting color
      float ndotl = normal.dot(lightdir);
      Vector3f Ldiff = data.HitSurface->Mat().Diffuse() * light->Intensity * std::max(0.0f, ndotl);

      // Specular Light calculations
      // Subtract the ray direction instead of add to reverse direction
      Vector3f halfdir = (lightdir - ray.direction).normalized();
      float ndoth = normal.dot(half);

      Vector3f Lspec = mat.specular() * light->Intensity * std::pow(std::max(0.0f, ndoth), mat.SpecularPower());

      out += Ldiff + Lspec;
    }
}

void RayTracer::sampleRate(int s)
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
            return glm::vec3(0.0f);
    }
}

Vector3f RayTracer::UniformSampler(int x, int y) const
{
    Vector3f result;
    float coef = 1.0f / SampleRate;
    for (float dy = 0.0f; dy < 1.0f; dy += coef)
    {
        for (float dy = 0.0f; dy < 1.0f; dyß += coef)
        {
            Ray ray = camera.getRay(x, y, dx, dy);
            result += trace(ray, 0);
        }
    }
    return result * coef * coef;
}

Vector3f RayTracer::RandomSampler(int x, int y) const
{
  // Random number generator for ray random sampling
  std::default_random_engine generator;
  std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

  Vector3f result;
  int s2 = SampleRate * SampleRate;
  for (int i = 0; i < s2; ++i)
  {
    float dx = distribution(generator);
    float dy = distribution(generator);

    Ray ray = camera.getRay(x, y, dx, dy);
    result += trace(ray, 0);
  }

  result = (result / (float) s2);

  return result;
}
