/**
 *  filename : main.cpp
 *  author   : Do Won Cha
 */

#include <GL/glew.h>
#include <GL/glut.h>

#include "RayTracer.h"
#include "Renderer.h"

int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowDisplayMode(100, 100);
    glutInitWindowSize(512, 512);
    glutCreateWindow("RayTracer");

    // Glew initialization
		glewExperimental = GL_TRUE;
		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
	  		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
			  exit(EXIT_FAILURE);
		}
		printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

    //glutReshapeFunc(Resize);
    //glutDisplayFunc(display);
    //glutKeyboardFunc(KeyboardFunc);
    //glutMouseFunc(MouseFunc);

    glutMainLoop();

    exit(EXIT_SUCCESS);
}
