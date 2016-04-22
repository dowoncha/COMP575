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

#include <list>
#include <algorithm>
#include <Eigen/Core>

#include "primitives/Mesh.hpp"
#include "primitives/Surface.hpp"
#include "Light.hpp"

struct HitData;

class Scene
{
public:
  Scene()
  {

  }

  Scene::~Scene()
  {
      for (Surface* s : surfaces)
      {
          delete s;
      }

      for (Light* l : Lights)
      {
          delete l;
      }
  }

  void addSurface(Surface* s)
  {
      surfaces.push_back(s);
  }

  void addLight(Light* l)
  {
      Lights.push_back(l);
  }

  HitData intersectSurfaces(const Ray& ray, float tMax, Surface* ignore = nullptr) const
  {
    // Intersect all surfaces using view ray
    // float tMin = tMax;
    HitData data;
    for (Surface* s : surfaces)
    {
      if (s == ignore)
        continue;

      if (s->Intersect(ray, data.t, data.tMax, data.tPoint))
      {
          data.point      = data.tPoint;
          data.t          = data.tMax;
          data.hitSurface = s;
      }
    }

    return data;
  }
private:
  std::list<Surface*> surfaces;
  std::list<Light*> lights;
  std::vector<KdNode> kdTree;
};

#endif /* end of header guard for _RAY_SCENE_ */
