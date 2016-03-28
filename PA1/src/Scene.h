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
    glm::vec3 Point, tPoint;
    glm::vec3 Normal;
    float t, tMax;
    Surface* HitSurface;
public:
    HitData() :
        tMax(100000.0f),
        HitSurface(nullptr)
{ }
};

 class Scene
 {
 public:
    Scene();
    ~Scene();

    void AddSurface(Surface* s);
    void AddLight(Light* l);

    bool IntersectSurfaces(const Ray& ray, float tMax, HitData& data) const;
    bool IntersectSurfaces(const Ray& ray, float tMax, const Surface* ignore) const;
public:
    std::list<Surface*> Surfaces;
    std::list<Light*> Lights;
 };

#endif /* end of header guard for _RAY_SCENE_ */
