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
#include <string>
#include <memory>

#include "ray.h"
#include "surface.h"
#include "light.h"
#include "material.h"

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
  void addSurface(const surface_t& surface);

  // Add a new light into the scene
  void addLight(const light_t& light);

  // Add a new material for any surface to use
  void addMaterial(const std::string& material_name, const material_t& material);

  /**
   *  Intersect ray with each surface in the list
   *  @param  ray to inersect surfaces with
   *  @param  hit data that returns info on the hit
   *  @return     Whether or not a surface was hit
   */
  bool intersectSurfaces(const Ray& ray, HitData& hit) const;

  // Read above. Just checks if a surface was hit, while taking a pointer to a surface to ignore checks.
  bool intersectSurfaces(const Ray& ray) const;
private:
  surfaces_list_t surfaces_;
  lights_list_t lights_;
  materials_map_t materials_;
};

} // end of namespace raytracer

#endif /* end of header guard for _RAY_SCENE_ */
