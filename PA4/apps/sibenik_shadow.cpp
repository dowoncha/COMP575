/**
 *  filename : main.cpp
 *  author   : Do Won Cha
 */

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <Eigen/Core>
#include <cstdio>
#include <cstring>
#include <string>
#include <memory>

#include "ray_tracer.h"
#include "scene.h"
#include "primitives/surface.h"
#include "primitives/material.h"
#include "primitives/light.h"
#include "primitives/surface_kd_mesh.h"

using namespace Eigen;
using namespace raytracer;

static RayTracer ray(512, 512);
static std::unique_ptr<Scene> scene(new Scene());

void display();

int main(int argc, char* argv[])
{
	// Glut initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(512, 512);
	glutCreateWindow("Sibenik");

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

	// White material with no specular power and no reflection
	std::shared_ptr<Material> white_mat(new Material(
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f),
		Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f)
	));

	scene->add_material("white", white_mat);

	scene->add_light(std::make_shared<Light>(
		Vector3f(0.0f, 0.0f, 0.0f),   // position
    Vector3f(1.0f, 1.0f, 1.0f),		// ambient
    Vector3f(1.0f, 1.0f, 1.0f)		// diffuse
	));

	scene->add_surface(std::make_shared<KdMesh>(
		"../assets/sibenik.obj",
		"../assets/sibenik.kd",
		white_mat
	));

	// No recursive trace, camera position, direction, shadows = true
	ray.set_max_trace_depth(1);
	ray.camera_->set_position(Vector3f(0.0f, -10.0f, 0.0f));
	ray.camera_->set_u(Vector3f(0.0f, 0.0f, 1.0f));
	ray.camera_->set_v(Vector3f(0.0f, 1.0f, 0.0f));
	ray.camera_->set_w(Vector3f(-1.0f, 0.0f, 0.0f));

	// render the scene
	ray.render(scene.get());

	glutDisplayFunc(display);
	glutMainLoop();

  exit(EXIT_SUCCESS);
}

void display()
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
