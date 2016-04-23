/**
 *
 *		filename : Scene.h
 *		author   : Do Won Cha
 *		content  : Scene container to hold surfaces and lights.
 *
 */

#pragma once
#ifndef _RAY_SCENE_
#define _RAY_SCENE_

#include <algorithm>
#include <unordered_map>
#include <list>
#include <memory>

#include "primitives/surface.hpp"
#include "primitives/light.hpp"
#include "primitives/material.hpp"

namespace raytracer
{

class Scene
{
  friend class RayTracer;

  typedef std::unordered_map<std::string, std::unique_ptr<Material>> materials_map_t;
  typedef std::list<std::unique_ptr<Surface>> surfaces_list_t;
  typedef std::list<std::unique_ptr<Light>> lights_list_t;

public:
  Scene()
  {}

  void add_surface(std::unique_ptr<Surface> surface)
  {
    surfaces_.push_back(std::move(surface));
  }
  void add_light(std::unique_ptr<Light> light)
  {
    // Move unique pointer into vector
    lights_.push_back(std::move(light));
  }
  void add_material(std::unique_ptr<Material> material, std::string material_name)
  {
    // The name of the material is used as its key
    materials_.emplace(material_name, std::move(material));
  }

  bool IntersectSurfaces(const Ray& ray, HitData& hit, Surface* ignore = nullptr)
  {
    // Intersect all surfaces using view ray
    for (const std::unique_ptr<Surface>& surface : surfaces_)
    {
      // If ignore optional pointer is passed through
      if (surface.get() == ignore)
        continue;

      // Surface is hit return true;
      if (surface->Intersect(ray, hit))
      {
          return true;
      }
    }

    return false;
  }
private:
  surfaces_list_t surfaces_;
  lights_list_t lights_;
  materials_map_t materials_;
};

} // end of namespace raytracer

#endif /* end of header guard for _RAY_SCENE_ */
