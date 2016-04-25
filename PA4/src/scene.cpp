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
