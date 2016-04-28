#include "scene.h"

using namespace raytracer;

Scene::Scene()
{}

Scene::~Scene()
{}

void Scene::add_surface(const surface_t& surface)
{
  surfaces_.push_back(std::move(surface));
}

void Scene::add_light(const light_t& light)
{
  // Move unique pointer into vector
  lights_.push_back(std::move(light));
}

void Scene::add_material(std::string material_name, const material_t& material)
{
  // The name of the material is used as its key
  materials_.insert({material_name, material});
}

bool Scene::intersect_surfaces(const Ray& ray, HitData& hit) const
{
  Vector3f point, normal;
  float min_distance = 100000.0f;
  for (const surface_t& surface : surfaces_)
  {
    if (surface->Intersect(ray, point, normal))
    {
      float distance = (hit.point - ray.position()).norm();
      if (distance < min_distance)
      {
        hit.surface = surface.get();
        hit.point = point;
        hit.normal = normal;
        min_distance = distance;
      }
    }
  }

  if (hit.surface != nullptr) return true;
  return false;
}

bool Scene::intersect_surfaces(const Ray& ray) const
{
  float min_distance = 100000.0f;
  for (const surface_t& surface : surfaces_)
  {
    if (surface->Intersect(ray))
    {
      return true;
    }
  }

  return false;
}
