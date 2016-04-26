/**
 *  filename : main.cpp
 *  author   : Do Won Cha
 */

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <float.h>

#include "primitives/surface.h"
#include "primitives/material.h"
#include "primitives/surface_kd_mesh.h"
#include "ray_tracer.h"
#include "scene.h"

void display();

static RayTracer ray;
static std::unique_ptr<Scene> scene(new Scene());

int main(int argc, char* argv[])
{
	std::shared_ptr<Material> white_mat(new Material(
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f),
		Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f),
	));

	scene->add_light(std::make_shared<Light>(
		Vector3f(0.0f, 0.0f, 0.0f),   // position
    Vector3f(1.0f, 1.0f, 1.0f),			//
    Vector3f(1.0f, 1.0f, 1.0f)
	));

	scene->add_surface(std::make_shared<KdMesh>(
		"../assets/sibenik.obj",
		"../assets/sibenik.kd",
		white_mat
	));

	// glut funcs
	ray.render(scene.get());

	glutDisplayFunc(Display);
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
