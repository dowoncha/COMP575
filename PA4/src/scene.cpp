#include "scene.h"

using namespace raytracer;

Scene::Scene()
{}

Scene::~Scene()
{}

void Scene::addSurface(const surface_t& surface)
{
  surfaces_.push_back(std::move(surface));
}

void Scene::addLight(const light_t& light)
{
  // Move unique pointer into vector
  lights_.push_back(std::move(light));
}

void Scene::addMaterial(std::string material_name, const material_t& material)
{
  // The name of the material is used as its key
  materials_.insert({material_name, material});
}

bool Scene::intersectSurfaces(const Ray& ray, HitData& hit) const
{
  // Intersect against all surfaces
  for (const surface_t& surface : surfaces_)
  {
    float t0 = hit.t;
    // Intesect against the surface and if the hit time is less than
    // pre intersection time, set the hit surface as the new surface.
    if (surface->intersect(ray, hit) && hit.t < t0)
    {
      hit.surface = surface.get();
    }
  }

  if (hit.surface != nullptr) return true;

  return false;
}

bool Scene::intersectSurfaces(const Ray& ray) const
{
  for (const surface_t& surface : surfaces_)
  {
    if (surface->Intersect(ray))
    {
      return true;
    }
  }

  return false;
}
