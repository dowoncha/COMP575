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
#include "material.h"
#include "plane.h"
#include "sphere.h"
#include "light.h"

static raytracer::RayTracer ray(512, 512);
static std::unique_ptr<raytracer::Scene> scene(new Scene());

void Idle();
void Display();

int main(int argc, char* argv[])
{
	using namespace Eigen;
	using namespace raytracer;

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

	auto red_mat = std::make_shared<Material>(
		Vector4f(0.2f, 0.0f, 0.0f, 1.0f),			// ambient
		Vector4f(1.0f, 0.0f, 0.0f, 1.0f),			// diffuse
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f)				// specular
	);

	auto green_mat = std::make_shared<Material>(
		Vector4f(0.0f, 0.2f, 0.0f, 1.0f),
		Vector4f(0.0f, 0.5f, 0.0f, 1.0f),
		Vector4f(0.5f, 0.5f, 0.5f, 1.0f),
		32.0f																		// specular power
	);

	auto blue_mat = std::make_shared<Material>(
		Vector4f(0.0f, 0.0f, 0.2f, 1.0f),
		Vector4f(0.0f, 0.0f, 1.0f, 1.0f),
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f),
		32.0f,																		// specular power
		0.8f																		// reflectivity
	);

	auto white_mat = std::make_shared<Material>(
		Vector4f(0.2f, 0.2f, 0.2f, 1.0f),
		Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f),
		0.0f,
		0.5f					// Reflectivity
	);

	auto sphere_red = std::make_shared<Sphere>(Vector3f(-4.0f, 0.0f, -7.0f), 1.0f, "red");
	auto sphere_blue = std::make_shared<Sphere>(Vector3f(4.0f, 0.0f, -7.0f), 1.0f, "blue");
	auto sphere_green = std::make_shared<Sphere>(Vector3f(0.0f, 0.0f, -7.0f), 2.0f, "green");

	auto plane_white = std::make_shared<Plane>(Vector3f(0.0f, -2.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), "white");

	scene->addMaterial("red", red_mat);
	scene->addMaterial("green", green_mat);
	scene->addMaterial("blue", blue_mat);
	scene->addMaterial("white", white_mat);

	// add 3 spheres(position, radius, material name) to the scene
	scene->addSurface(sphere_red);
	scene->addSurface(sphere_green);
	scene->addSurface(sphere_blue);

	// Add flat white plane to the scene.
	scene->addSurface(plane_white);

	// Add a light to the scene, default intensity = 1.0f
	scene->addLight(std::make_shared<Light>(
		Vector3f(-4.0f, 4.0f, -3.0f),   // position
    Vector3f(1.0f, 1.0f, 1.0f),			// ambient
    Vector3f(1.0f, 1.0f, 1.0f)  		// diffuse
	));

	// Set the max trace depth to 2
	// and pass a raw scene pointer to the renderer.
	ray.setMaxTraceDepth(2);
	ray.render(scene.get());

	// glut callbacks
	glutDisplayFunc(Display);
	glutMainLoop();

  exit(EXIT_SUCCESS);
}

void Display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// GL2.0 draw the raytracer
  glDrawPixels(512,
               512,
               GL_RGBA,
               GL_FLOAT,
               ray.frameBuffer());

  glutSwapBuffers();
  glutPostRedisplay();
}
