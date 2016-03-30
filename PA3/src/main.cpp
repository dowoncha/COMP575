#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glext.h>

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

#define CALL_MEMBER_FN(object, ptrToMember)  ((object).*(ptrToMember))

/*
	How to use this code:

	Call init_timer before starting rendering, i.e., before calling
	glutMainLoop. Then, make sure your display function is organized
	roughly as the example below.
*/

// Bunny
Mesh bunny("bunny.obj");

// Lighting
static GLfloat lightPos[] = {0.0f, 0.0f, 0.0f, 0.0f};
static GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
static GLfloat white[] = 	 {1.0f, 1.0f, 1.0f, 1.0f};
static GLfloat black[] = 	 {0.0f, 0.0f, 0.0f, 1.0f};
static glm::vec3 lightDir = glm::normalize(glm::vec3(-1.0f));

// Timing
float  					gTotalTimeElapsed 	= 0;
int 					 gTotalFrames		= 0;
GLuint 				 gTimer;

void initGL()
{
	// Setup depth buffer, shading, and culling
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH);
	//glCullFace(GL_BACK);

	// Setup lightings
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// Load view matrices onto initial projection stack.
	glViewport(0.0f, 0.0f, 512.0f, 512.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.1f, 0.1f, -0.1f, 0.1f, 0.1f, 1000.0f);

	// set matrix mode back to model
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Set eye
	gluLookAt(0.0f, 0.0f, 0.0f,					// eye position
					 0.0f, 0.0f, -1.0f,				   // Target
				     0.0f, 1.0f, 0.0f);				   // Up vector
}

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
	else if(key == 112) // q
	{
		lightPos[2] -= 1.0f;
		printf("Light pos z: %f\n", lightPos[2]);
	}
}

void SetLighting()
{
	// GL default: Initial ambient is 0.2 0.2 0.2 1.0
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    // Add directed light here
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, black	); 			  // Default ambient is 0
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);   			// Default diffuse is 1
	glLightfv(GL_LIGHT0, GL_SPECULAR, black); 			 // Default for light0 is 1 so set to 0
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, glm::value_ptr(lightDir));
}


/**
 *  Your display function should look roughly like the following.
*/
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
 	start_timing();

	SetLighting();

	CALL_MEMBER_FN(bunny, bunny.render)();

	float timeElapsed = stop_timing();
  	gTotalFrames++;
  	gTotalTimeElapsed += timeElapsed;
  	float fps = gTotalFrames / gTotalTimeElapsed;
  	char string[1024] = {0};
  	sprintf(string, "OpenGL Bunny: %0.2f FPS", fps);
  	glutSetWindowTitle(string);

	glutPostRedisplay();
  	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	// Glut initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(512, 512);
    glutCreateWindow("GL");

	// Glew initialization
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
  		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		exit(EXIT_FAILURE);
	}
	printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

	initGL();
	init_timer();

	bunny.SetupMesh();

	glutReshapeFunc(Resize);
    glutDisplayFunc(display);
	glutKeyboardFunc(KeyboardFunc);
	glutMouseFunc(MouseFunc);

    glutMainLoop();

    exit(EXIT_SUCCESS);
}
