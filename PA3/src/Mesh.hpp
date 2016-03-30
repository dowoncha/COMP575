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
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh;

struct Vector3
{
	float x, y, z;
};

struct Vertex
{
	glm::vec3 pos, normal;
};

struct Triangle
{
	unsigned int indices[3];
};

typedef void (Mesh::*RenderFunction)();

class Mesh
{
public:
    std::vector<Vector3>	gPositions;
    std::vector<Vector3>	gNormals;
    std::vector<Triangle>	 gTriangles;
	std::vector<Vertex> 	  gVertices;

	// Transforms
	glm::vec3 pos, scale;

	// Materials
	glm::vec4 ambient, diffuse, specular;
	float specPower;

	RenderFunction render;
private:
	GLuint vao, ebo;
	GLuint vbo[2];

	bool Immediate;
public:
	Mesh(std::string filename) :
		pos(0.1f, -1.0f, -1.5f),
		scale(10.0f),
		ambient(1.0f),
		diffuse(1.0f),
		specular(0.0f),
		specPower(0.0f),
		Immediate(true),
		render(&Mesh::RenderImmediate)
	{
		load_mesh(filename);
		SetupMesh();
	}

	~Mesh()
	{}

	void SetupMesh()
	{
    	glGenVertexArrays(1, &this->vao);
    	glGenBuffers(1, &this->vbo[0]);
		glGenBuffers(1, &vbo[1]);
    	glGenBuffers(1, &ebo);

		printf("Bind vao and buffer\n");
    	glBindVertexArray(vao);
    		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, gPositions.size() * sizeof(Vector3), &gPositions[0], GL_STATIC_DRAW);

    		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    		glBufferData(GL_ELEMENT_ARRAY_BUFFER, gTriangles.size() * sizeof(Triangle), &gTriangles[0], GL_STATIC_DRAW);

    		// Vertex Positions
    		glEnableVertexAttribArray(0);
    		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (GLvoid*) 0);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, gNormals.size() * sizeof(Vector3), &gNormals[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
    		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3), (GLvoid*) 0);
		glBindVertexArray(0);
	}

	void ToggleRenderMethod()
	{
		Immediate = !Immediate;
		if (Immediate)
		{
			render = &Mesh::RenderImmediate;
		}
		else
		{
			render = &Mesh::RenderBuffer;
		}
	}

	void RenderImmediate()
	{
		glPushMatrix();

		// Set transforms and materials of mesh
		glTranslatef(pos.x, pos.y, pos.z);
		glScalef(scale.x, scale.y, scale.z);

		// Set material values
		glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(ambient));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(diffuse));
		glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(specular));
		glMaterialfv(GL_FRONT, GL_SHININESS, &specPower);
		glColor4fv(glm::value_ptr(ambient));

	    // Load vertices into gpu
	    glBegin(GL_TRIANGLES);
	        for (const Triangle& t : gTriangles)
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
		glPopMatrix();
	}

	void RenderBuffer()
	{
		glPushMatrix();

		// Set transforms and materials of mesh
		glTranslatef(pos.x, pos.y, pos.z);
		glScalef(scale.x, scale.y, scale.z);

		// Set material values
		glMaterialfv(GL_FRONT, GL_AMBIENT, glm::value_ptr(ambient));
		glMaterialfv(GL_FRONT, GL_DIFFUSE, glm::value_ptr(diffuse));
		glMaterialfv(GL_FRONT, GL_SPECULAR, glm::value_ptr(specular));
		glMaterialfv(GL_FRONT, GL_SHININESS, &specPower);
		glColor4fv(glm::value_ptr(ambient));

		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, gTriangles.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glPopMatrix();
	}
private:
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

	static void tokenize(char* string, std::vector<std::string>& tokens, const char* delimiter)
	{
		char* token = strtok(string, delimiter);
		while (token != NULL)
		{
			tokens.push_back(std::string(token));
			token = strtok(NULL, delimiter);
		}
	}

	static int face_index(const char* string)
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
};
