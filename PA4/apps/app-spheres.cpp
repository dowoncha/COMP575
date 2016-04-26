/**
 *  filename : main.cpp
 *  author   : Do Won Cha
 *  content  : Ray trace 3 spheres and a plane with reflection
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Eigen/Core>
#include <cstdio>
#include <cstring>
#include <string>
#include <memory>

#include "scene.h"
#include "ray_tracer.h"
#include "primitives/material.h"
#include "primitives/surface_plane.h"
#include "primitives/surface_sphere.h"
#include "primitives/light.h"

using namespace Eigen;
using namespace raytracer;

static RayTracer ray;
static std::unique_ptr<Scene> scene(new Scene());

void Idle();
void Display();

int main(int argc, char* argv[])
{
	// Glut initialization
	glutInit(&argc, argv);
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

	//std::unique_ptr<Scene> scene(new Scene());

	std::shared_ptr<Material> red_mat(new Material(
		Vector4f(0.2f, 0.0f, 0.0f, 1.0f),			// ambient
		Vector4f(1.0f, 0.0f, 0.0f, 1.0f),			// diffuse
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f)				// specular
	));

	std::shared_ptr<Material> green_mat(new Material(
		Vector4f(0.0f, 0.2f, 0.0f, 1.0f),
		Vector4f(0.0f, 0.5f, 0.0f, 1.0f),
		Vector4f(0.5f, 0.5f, 0.5f, 1.0f),
		32.0f																		// specular power
	));

	std::shared_ptr<Material> blue_mat(new Material(
		Vector4f(0.0f, 0.0f, 0.2f, 1.0f),
		Vector4f(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f),
		32.0f,																		// specular power
		0.8f																		// reflectivity
	));

	std::shared_ptr<Material> white_mat(new Material(
		Vector4f(0.2f, 0.2f, 0.2f, 1.0f),
		Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f),
		0.0f,
		0.5f					// Reflectivity
	));

	std::shared_ptr<Surface> sphere_red(new Sphere(Vector3f(-4.0f, 0.0f, -7.0f), 1.0f, red_mat));
	std::shared_ptr<Surface> sphere_blue(new Sphere(Vector3f(4.0f, 0.0f, -7.0f), 1.0f, blue_mat));
	std::shared_ptr<Surface> sphere_green(new Sphere(Vector3f(0.0f, 0.0f, -7.0f), 2.0f, green_mat));

	std::shared_ptr<Surface> plane_white(new Plane(Vector3f(0.0f, -2.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), white_mat));

	scene->add_material("red", red_mat);
	scene->add_material("green", green_mat);
	scene->add_material("blue", blue_mat);
	scene->add_material("white", white_mat);

	// add 3 spheres(position, radius, material name) to the scene
	scene->add_surface(sphere_red);
	scene->add_surface(sphere_blue);
	scene->add_surface(sphere_green);

	// Add flat white plane to the scene.
	scene->add_surface(plane_white);

	// Add a light to the scene, default intensity = 1.0f
	scene->add_light(std::make_shared<Light>(
		Vector3f(-4.0f, 4.0f, -3.0f),   // position
    Vector3f(1.0f, 1.0f, 1.0f),			// ambient
    Vector3f(1.0f, 1.0f, 1.0f)  		// diffuse
	));

	ray.render(scene.get());

	glutDisplayFunc(Display);
	glutMainLoop();

  exit(EXIT_SUCCESS);
}

void Display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glDrawPixels(512,
               512,
               GL_RGBA,
               GL_FLOAT,
               ray.frame_buffer_.data());

  glutSwapBuffers();
  glutPostRedisplay();
}
