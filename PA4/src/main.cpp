/**
 *  filename : main.cpp
 *  author   : Do Won Cha
 */

#include "RayTracer.h"
#include "Renderer.h"

/**
 * filename: main.cpp
 * author : Do Won Cha
 * content : Start point to render a bunny. Handles all gl initialization
 */

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <float.h>

// GLM
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.hpp"

// This is to call a member function pointer, dirty but quick
#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember))

/*
	How to use this code:

	Call init_timer before starting rendering, i.e., before calling
	glutMainLoop. Then, make sure your display function is organized
	roughly as the example below.
*/

// Lighting
static GLfloat lightPos[] = {30.0f, 0.0f, 20.0f, 0.0f};
static GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
static GLfloat white[] = 	 {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat black[] = 	 {0.0f, 0.0f, 0.0f, 1.0f};
static GLfloat lightDir[] = {-1.0f, -1.0f, -1.0f};

// Timing
float  					gTotalTimeElapsed 	= 0;
int 					 gTotalFrames		= 0;
GLuint 				 gTimer;

void init_timer()
{
	printf("Initializing Timer\n");
	glGenQueries(1, &gTimer);
}

void start_timing()
{
	glBeginQuery(GL_TIME_ELAPSED, gTimer);
}

float stop_timing()
{
	glEndQuery(GL_TIME_ELAPSED);

	GLint available = GL_FALSE;
	while (available == GL_FALSE)
		glGetQueryObjectiv(gTimer, GL_QUERY_RESULT_AVAILABLE, &available);

	GLint result;
	glGetQueryObjectiv(gTimer, GL_QUERY_RESULT, &result);

	float timeElapsed = result / (1000.0f * 1000.0f * 1000.0f);
	return timeElapsed;
}

void Resize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	if(h == 0) h = 1;

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);
	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    glFrustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);

	// set matrix mode back to model
	glMatrixMode(GL_MODELVIEW);
}

void MouseFunc(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		printf("Mouse left pressed: x: %d, y: %d\n", x, y);
	}
}

void KeyboardFunc(unsigned char key, int x, int y)
{
	if (key == 116)
	{
		bunny.ToggleRenderMethod();
	}
	else if(key  == 119)	// W
	{
		lightPos[1] += 1.0f;
		printf("W pressed %f\n", lightPos[1]);
	}
	else if(key == 115)
	{
		lightPos[1] -= 1.0f;
		printf("Light pos y changed: %f\n", lightPos[1]);
	}
	else if(key == 97)
	{
		lightPos[0] -= 1.0f;
		printf("Light pos x: %f\n", lightPos[0]);
	}
	else if(key == 100) // d
	{
		lightPos[0] += 1.0f;
		printf("Light pos x: %f\n", lightPos[0]);
	}
	else if( key == 101) // e
	{
		lightPos[2] += 1.0f;
		printf("Light pos z: %f\n", lightPos[2]);
	}
	else if(key == 113) // q
	{
		lightPos[2] -= 1.0f;
		printf("Light pos z: %f\n", lightPos[2]);
	}
}

int main(int argc, char* argv[])
{
		init_timer();

		glutReshapeFunc(Resize);
	  glutDisplayFunc(display);
		glutKeyboardFunc(KeyboardFunc);
		glutMouseFunc(MouseFunc);

    glutMainLoop();

    exit(EXIT_SUCCESS);
}


int main(int argc, char* argv[])
{
  // Make surfaces
  Sphere sphere1( Vector3f(0.0f, 0.0f, -7.0f), 2.0f, green);
  Sphere sphere2( Vector3f(-4.0f, 0.0f, -7.0f), 1.0f, red);
  Sphere sphere3( Vector3f(4.0f, 0.0f, -7.0f), 1.0f, blue);

  // Make the flat plane
  Plane plane(Vector3f(0.0f, -2.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f), white);

  // Make lights
  Light light(Vector3f(-4.0f, 4.0f, -3.0f),   //
              Vector3f(1.0f, 1.0f, 1.0f),
              Vector3f(1.0f, 1.0f, 1.0f));
  light1.SetIntensity(1.0f);

  Scene scene;
  scene.AddLight(&light1);
  scene.AddSurface(&plane);
  scene.AddSurface(&sphere1);
  scene.AddSurface(&sphere2);
  scene.AddSurface(&sphere3);

  RayTracer ray;
  ray.addScene();
  ray.run();

  exit(EXIT_SUCCESS);
}
