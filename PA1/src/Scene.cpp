#include "Scene.h"

Scene::Scene()
{
}

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

bool Scene::IntersectSurfaces(const Ray& ray, float tMax, HitData& data) const
{
    bool surfacehit = false;
    // Intersect all surfaces using view ray
    // float tMin = tMax;
    for (Surface* s : Surfaces)
    {
        if (s->Intersect(ray, data.t, data.tMax, data.tPoint))
        {
            data.Point      = data.tPoint;
            data.t          = data.tMax;
            data.HitSurface = s;
            data.Normal     = data.HitSurface->GetNormal(data.Point);
            surfacehit = true;
        }
    }

    return surfacehit;
}

bool Scene::IntersectSurfaces(const Ray& ray, float tMax, const Surface *ignore) const
{
    // Intersect all surfaces using view ray
    for (Surface* s : Surfaces)
    {
        if (s == ignore) continue;

        if (s->Intersect(ray, tMax))
        {
            return true;
        }
    }

    return false;
}
