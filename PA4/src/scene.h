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

#include "primitives/surface.h"
#include "primitives/light.h"
#include "primitives/material.h"

namespace raytracer
{

class Scene
{
  friend class RayTracer;

  using materials_map_t = std::unordered_map<std::string, material_t>;
  using surfaces_list_t = std::list<surface_t>;
  using lights_list_t   = std::list<light_t>;
public:
  Scene();

  ~Scene();

  // Add a new surface in the scene
  void add_surface(const surface_t& surface);

  // Add a new light into the scene
  void add_light(const light_t& light);

  // Add a new material for any surface to use
  void add_material(std::string material_name, const material_t& material);

  bool intersect_surfaces(const Ray& ray, HitData& hit);

  // ignore defaults to nullptr
  bool intersect_surfaces(const Ray& ray, Surface* ignore);
private:
  surfaces_list_t surfaces_;
  lights_list_t lights_;
  materials_map_t materials_;
};

} // end of namespace raytracer

#endif /* end of header guard for _RAY_SCENE_ */
