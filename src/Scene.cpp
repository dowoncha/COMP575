#include "Scene.h"

Scene::Scene()
{
    SceneInit();
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

void Scene::SceneInit()
{
    Material* red = Material::CreateRedMat();
    Material* green = Material::CreateGreenMat();
    Material* blue = Material::CreateBlueMat();
    Material* planeMat = Material::CreateWhiteMat();

    Sphere* sphere1 = new Sphere( Vector3f(-4.0f, 0.0f, -7.0f), 1.0f, red);
    Sphere* sphere2 = new Sphere( Vector3f(0.0f, 0.0f, -7.0f), 2.0f, green);
    Sphere* sphere3 = new Sphere( Vector3f(4.0f, 0.0f, -7.0f), 1.0f, blue);
    Sphere* sphere4 = new Sphere( Vector3f(0.0f, 0.0f, -19.0f), 10.0f, blue);

    Plane* plane = new Plane(Vector3f(0.0f, -2.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), planeMat);

    Surfaces.push_back(plane);
    //Surfaces.push_back(sphere4);
    Surfaces.push_back(sphere1);
    Surfaces.push_back(sphere2);
    Surfaces.push_back(sphere3);

    Light* light1 = new Light(Vector3f(-4.0f, 4.0f, -3.0f), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(1.0f, 1.0f, 1.0f));
    Light* light2 = new Light(Vector3f(0.0f, -5.0f, -4.0f), Vector3f(1.0f, 1.0f, 1.0f), Vector3f(1.0f, 1.0f, 1.0f));

    light1->SetIntensity(1.0f);

    Lights.push_back(light1);
    //Lights.push_back(light2);

    // sort by z value
    Surfaces.sort([] (const Surface* a, const Surface* b) -> bool {
        return a->GetPosition().z < b->GetPosition().z;
    });
}

bool Scene::IntersectSurfaces(const Ray& ray, float tMax, HitData& data) const
{
    bool surfacehit = false;
    // Intersect all surfaces using view ray
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

bool Scene::IntersectSurfaces(const Ray& ray, float tMax, Surface *ignore) const
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
