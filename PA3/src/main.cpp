/*
	How to use this code:

	Call load_mesh("bunny.obj") after creating the window using GLUT. This will
	read the mesh data from bunny.obj and populate the arrays gPositions,
	gNormals, and gTriangles.

	When rendering, we use a similar convention to the sphere from PA2. In other
	words, for triangle i, define:

		k0 = gTriangles[i].indices[0];
		k1 = gTriangles[i].indices[1];
		k2 = gTriangles[i].indices[2];

	Then the vertices of the triangle are at gPositions[k0], gPositions[k1], and
	gPositions[k2], in that order. The normals of the corresponding vertices are
	at gNormals[k0], gNormals[k1], and gNormals[k2], in that order.

	For the second part of the assignment, you have to copy the gPositions,
	gNormals, and gTriangles arrays into GPU memory.
*/

#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <float.h>

struct Vector3
{
	float			x, y, z;
};

struct Triangle
{
	unsigned int 	indices[3];
};

std::vector<Vector3>	gPositions;
std::vector<Vector3>	gNormals;
std::vector<Triangle>	gTriangles;

void tokenize(char* string, std::vector<std::string>& tokens, const char* delimiter)
{
	char* token = strtok(string, delimiter);
	while (token != NULL)
	{
		tokens.push_back(std::string(token));
		token = strtok(NULL, delimiter);
	}
}

int face_index(const char* string)
{
	int length = strlen(string);
	char* copy = new char[length + 1];
	memset(copy, 0, length+1);
	strcpy(copy, string);

	std::vector<std::string> tokens;
	tokenize(copy, tokens, "/");
	delete[] copy;
	if (tokens.front().length() > 0 && tokens.back().length() > 0 && atoi(tokens.front().c_str()) == atoi(tokens.back().c_str()))
	{
		return atoi(tokens.front().c_str());
	}
	else
	{
		printf("ERROR: Bad face specifier!\n");
		exit(0);
	}
}

void load_mesh(std::string fileName)
{
	std::ifstream fin(fileName.c_str());
	if (!fin.is_open())
	{
		printf("ERROR: Unable to load mesh from %s!\n", fileName.c_str());
		exit(0);
	}

	float xmin = FLT_MAX;
	float xmax = -FLT_MAX;
	float ymin = FLT_MAX;
	float ymax = -FLT_MAX;
	float zmin = FLT_MAX;
	float zmax = -FLT_MAX;

	while (true)
	{
		char line[1024] = {0};
		fin.getline(line, 1024);

		if (fin.eof())
			break;

		if (strlen(line) <= 1)
			continue;

		std::vector<std::string> tokens;
		tokenize(line, tokens, " ");

		if (tokens[0] == "v")
		{
			float x = atof(tokens[1].c_str());
			float y = atof(tokens[2].c_str());
			float z = atof(tokens[3].c_str());

			xmin = std::min(x, xmin);
			xmax = std::max(x, xmax);
			ymin = std::min(y, ymin);
			ymax = std::max(y, ymax);
			zmin = std::min(z, zmin);
			zmax = std::max(z, zmax);

			Vector3 position = {x, y, z};
			gPositions.push_back(position);
		}
		else if (tokens[0] == "vn")
		{
			float x = atof(tokens[1].c_str());
			float y = atof(tokens[2].c_str());
			float z = atof(tokens[3].c_str());
			Vector3 normal = {x, y, z};
			gNormals.push_back(normal);
		}
		else if (tokens[0] == "f")
		{
			unsigned int a = face_index(tokens[1].c_str());
			unsigned int b = face_index(tokens[2].c_str());
			unsigned int c = face_index(tokens[3].c_str());
			Triangle triangle;
			triangle.indices[0] = a - 1;
			triangle.indices[1] = b - 1;
			triangle.indices[2] = c - 1;
			gTriangles.push_back(triangle);
		}
	}

	fin.close();

	printf("Loaded mesh from %s. (%lu vertices, %lu normals, %lu triangles)\n", fileName.c_str(), gPositions.size(), gNormals.size(), gTriangles.size());
	printf("Mesh bounding box is: (%0.4f, %0.4f, %0.4f) to (%0.4f, %0.4f, %0.4f)\n", xmin, ymin, zmin, xmax, ymax, zmax);
}

/*
	How to use this code:

	Call init_timer before starting rendering, i.e., before calling
	glutMainLoop. Then, make sure your display function is organized
	roughly as the example below.
*/

float  					gTotalTimeElapsed 	= 0;
int 					gTotalFrames		= 0;
GLuint 					gTimer;

void init_timer()
{
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

/*
	Your display function should look roughly like the following.
*/
void display()
{
	// TODO: Clear the screen and depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

 	start_timing();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //glTrasnlatef();

    // Set the ambient light
    GLfloat ambient[] = {0.2f, 0.2f, 0.2f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    // Add directed light here

    // TODO: Draw the bunny.
    glBegin(GL_TRIANGLES);
        for (Triangle t : gTriangles)
        {
            int k0 = t.indices[0];
            int k1 = t.indices[1];
            int k2 = t.indices[2];

            glNormal3f(gNormals[k0].x, gNormals[k0].y, gNormals[k0].z);
            glVertex3f(gPositions[k0].x, gPositions[k0].y, gPositions[k0].z);
            glNormal3f(gNormals[k1].x, gNormals[k1].y, gNormals[k1].z);
            glVertex3f(gPositions[k1].x, gPositions[k1].y, gPositions[k1].z);
            glNormal3f(gNormals[k2].x, gNormals[k2].y, gNormals[k2].z);
            glVertex3f(gPositions[k2].x, gPositions[k2].y, gPositions[k2].z);
        }
    glEnd();

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
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(512, 512);
    glutCreateWindow("GL");

    load_mesh("bunny.obj");

    glutDisplayFunc(display);

    glutMainLoop();

    exit(EXIT_SUCCESS);
}
