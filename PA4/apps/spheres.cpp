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

using namespace Eigen;
using namespace raytracer;

std::unique_ptr<Scene> scene;
std::unique_ptr<RayTracer> ray;

void Idle()
{
	ray->Render();
}

void Display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glDrawPixels(camera_->screen_width(),
               camera_->screen_height(),
               GL_RGBA,
               GL_FLOAT,
               ray->frame_buffer_.data());

  glutSwapBuffers();
  glutPostRedisplay();
}

int main(int argc, char* argv[])
{
	// Glut initialization
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Ray Tracer");

	// Glew initialization
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	printf("Status: GLEW %s\n", glewGetString(GLEW_VERSION));

	// glut funcs
	glutIdleFunc(Idle);
	glutDisplayFunc(Display);

	scene = std::make_unique<Scene>();

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

  ray = std::make_unique<RayTracer>(&argc, argv);
	ray->initialize(scene);

	glutMainLoop();

  exit(EXIT_SUCCESS);
}
