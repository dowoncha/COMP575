/**
 *  filename : Renderer.h
 *  content  : GL rendering code
 */

#pragma once
#ifndef _RAY_RENDERER_
#define _RAY_RENDERER_

#include <GL/glew.h>
#include <GL/glut.h>

class Renderer
{
public:
    Renderer(int* argc, char** argv) : ray(_ray)
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
  		printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

  		initGL();
    }

    ~Renderer();

    void initGL()
    {
    	// Setup depth buffer, shading, and culling
    	glEnable(GL_DEPTH_TEST);
    	glEnable(GL_CULL_FACE);
    	//glDisable(GL_CULL_FACE);
    	glDepthFunc(GL_LESS);
    	glShadeModel(GL_SMOOTH);
    	glEnable(GL_RESCALE_NORMAL);

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

    void display()
    {
    	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
     	start_timing();

    	scene.render();

    	float timeElapsed = stop_timing();
    	gTotalFrames++;
    	gTotalTimeElapsed += timeElapsed;
    	float fps = gTotalFrames / gTotalTimeElapsed;
    	char string[1024] = {0};
    	sprintf(string, "Ray tracer FPS: %0.2f", fps);
    	glutSetWindowTitle(string);

    	glutPostRedisplay();
      glutSwapBuffers();
    }

    /**
     *  Render ray traced image using gl
     */
    void render()
    {

    }
private:
  const RayTracer& ray;
};

#endif _RAY_RENDERER_
