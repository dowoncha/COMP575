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

#include "primitives/Surface.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
#include "RayTracer.h"

// This is to call a member function pointer, dirty but quick
#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember))

int main(int argc, char* argv[])
{
	std::shared_ptr<Material> white_mat(new Material(
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f),
		Vector4f(1.0f, 1.0f, 1.0f, 1.0f),
		Vector4f(0.0f, 0.0f, 0.0f, 1.0f),
	));

  // Make light
  std::shared_ptr<Light> light(new Light(
		Vector3f(0.0f, 0.0f, 0.0f),   // position
    Vector3f(1.0f, 1.0f, 1.0f),			//
    Vector3f(1.0f, 1.0f, 1.0f)
	);

  Scene scene;
  scene.AddLight(&light);
  scene.loadMesh("../assets/sibenik2.obj");
  scene.loadKdTree("../assets/kdtree2.simple");

	// glut funcs
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
