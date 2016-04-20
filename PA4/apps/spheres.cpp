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

#include "Surface.h"
#include "Material.h"
#include "Mesh.hpp"
#include "RayTracer.h"

// This is to call a member function pointer, dirty but quick
#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember))

int main(int argc, char* argv[])
{
	Material red = Material::CreateRedMat();
	Material green = Material::CreateWhiteMat();
	green.specPower(32.0f);
	Material blue = Material::CreateBlueMat();
	blue.reflection(0.8f);
	Material white = Material::CreateWhiteMat();
	white.reflection(0.5f);

  // Make surfaces
	Sphere sphere1( Vector3f(-4.0f, 0.0f, -7.0f), 1.0f, red);
  Sphere sphere2( Vector3f(0.0f, 0.0f, -7.0f), 2.0f, green);
  Sphere sphere3( Vector3f(4.0f, 0.0f, -7.0f), 1.0f, blue);

  // Make the flat plane
  Plane plane(Vector3f(0.0f, -2.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), white);

  // Make light
  Light light(Vector3f(-4.0f, 4.0f, -3.0f),   // position
              Vector3f(1.0f, 1.0f, 1.0f),			//
              Vector3f(1.0f, 1.0f, 1.0f));		//
  light.intensity(1.0f);

  Scene scene;
  scene.AddLight(&light1);
  scene.AddSurface(&plane);
  scene.AddSurface(&sphere1);
  scene.AddSurface(&sphere2);
  scene.AddSurface(&sphere3);

  RayTracer ray(&argc, argv);
	ray.init(scene);
  ray.run();

  exit(EXIT_SUCCESS);
}
