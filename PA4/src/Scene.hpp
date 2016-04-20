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

#include <glm/vec3.hpp>

#include "Surface.h"
#include "Light.h"

/**
 *	Hit data is returned upon call to IntersectSurfaces.
 */
class HitData
{
public:
  Vector3f hitPoint, hitTime;
  Vector3f normal;
  float t, tMax;
  Surface* HitSurface;
public:
  HitData() :
    tMax(100000.0f),
    t(0),
    HitSurface(nullptr)
  { }
};

class Scene
{
public:
  Scene();

  Scene::~Scene()
  {
      for (Surface* s : Surfaces)
      {
          delete s;
      }

      for (Light* l : Lights)
      {
          delete l;
      }
  }

  void Scene::AddSurface(Surface* s)
  {
      Surfaces.push_back(s);
  }

  void Scene::AddLight(Light* l)
  {
      Lights.push_back(l);
  }

  void AddMesh(Mesh* m);

  HitData Scene::IntersectSurfaces(const Ray& ray, float tMax, Surface* ignore = nullptr) const
  {
        // Intersect all surfaces using view ray
        // float tMin = tMax;
        HitData data;
        for (Surface* s : Surfaces)
        {
          if (s == ignore)
            continue;

          if (s->Intersect(ray, data.t, data.tMax, data.tPoint))
          {
              data.Point      = data.tPoint;
              data.t          = data.tMax;
              data.HitSurface = s;
              data.Normal     = data.HitSurface->GetNormal(data.Point);
          }
        }

        return data;
  }

private:
  std::list<Surface*> Surfaces;
  std::list<Light*> Lights;
};

#endif /* end of header guard for _RAY_SCENE_ */
