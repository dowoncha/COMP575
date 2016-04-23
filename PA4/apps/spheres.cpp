/**
 *  filename : main.cpp
 *  author   : Do Won Cha
 *  content  : Ray trace 3 spheres and a plane with reflection
 */

#include <Eigen/Core>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <memory>

#include "scene.hpp"
#include "ray_tracer.h"
#include "primitives/material.hpp"
#include "primitives/surface_plane.hpp"
#include "primitives/surface_sphere.hpp"
#include "primitives/light.hpp"

int main(int argc, char* argv[])
{
	using namespace Eigen;
	using namespace raytracer;

	std::unique_ptr<Scene> scene(new Scene());

	scene->add_material(std::make_unique<Material>(
		Vector4f(0.2f, 0.0f, 0.0f, 1.0f),			// ambient
		Vector4f(1.0f, 0.0f, 0.0f, 1.0f),			// diffuse
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f)				// specular
	), "red");

	scene->add_material(std::make_unique<Material>(
		Vector4f(0.0f, 0.2f, 0.0f, 1.0f),
		Vector4f(0.0f, 0.5f, 0.0f, 1.0f),
		Vector4f(0.5f, 0.5f, 0.5f, 1.0f),
		32.0f																		// specular power
	), "green");

	scene->add_material(std::make_unique<Material>(
		Vector4f(0.0f, 0.0f, 0.2f, 1.0f),
		Vector4f(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f),
		0.0f,																		// specular power
		32.0f																		// reflectivity
	), "blue");

	scene->add_material(std::make_unique<Material>(
		Vector4f(0.2f, 0.2f, 0.2f, 1.0f),
		Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f),
		0.0f,
		0.5f					// Reflectivity
	), "white");

	// add 3 spheres(position, radius, material name) to the scene
	scene->add_surface(std::make_unique<Sphere>(Vector3f(-4.0f, 0.0f, -7.0f), 1.0f, "red"));
	scene->add_surface(std::make_unique<Sphere>(Vector3f(0.0f, 0.0f, -7.0f), 2.0f, "green"));
	scene->add_surface(std::make_unique<Sphere>(Vector3f(4.0f, 0.0f, -7.0f), 1.0f, "blue"));

	// Add flat white plane to the scene.
	scene->add_surface(std::make_unique<Plane>(Vector3f(0.0f, -2.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), "white"));

	// Add a light to the scene, default intensity = 1.0f
	scene->add_light(std::make_unique<Light>(
		Vector3f(-4.0f, 4.0f, -3.0f),   // position
    Vector3f(1.0f, 1.0f, 1.0f),			// ambient
    Vector3f(1.0f, 1.0f, 1.0f)  		// diffuse
	));

  std::unique_ptr<RayTracer> ray(new RayTracer(&argc, argv));
	ray->initialize(scene);
  ray->run();

  exit(EXIT_SUCCESS);
}
