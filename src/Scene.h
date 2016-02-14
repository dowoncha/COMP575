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

#include "Primitives.h"
#include "Surface.h"
#include "Light.h"

/**
 *	Hit data is returned upon call to IntersectSurfaces.
 */
class HitData
{
public:
    Vector3f Point, tPoint;
    Vector3f Normal;
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

    void SceneInit();

    bool IntersectSurfaces(const Ray& ray, float tMax, HitData& data) const;
    bool IntersectSurfaces(const Ray& ray, float tMax, Surface* ignore) const;
public:
    std::list<Surface*> Surfaces;
    std::list<Light*> Lights;
 };

#endif /* end of header guard for _RAY_SCENE_ */
