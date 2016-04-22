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
	Material white;
  white.ambient(Vector3f(0.0f));
  white.diffuse(Vector3f(1.0f));
  white.specular(Vector3f(0.0f));

  // Make light
  Light light(Vector3f(0.0f, 0.0f, 0.0f),   // position
              Vector3f(1.0f, 1.0f, 1.0f),			//
              Vector3f(1.0f, 1.0f, 1.0f));		//
  light.intensity(1.0f);

  Scene scene;
  scene.AddLight(&light);
  scene.loadMesh("../assets/sibenik2.obj");
  scene.loadKdTree("../assets/kdtree2.simple");

  RayTracer ray(&argc, argv);
	ray.init(scene);
  ray.run();

  exit(EXIT_SUCCESS);
}
