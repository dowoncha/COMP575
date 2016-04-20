/**
 *  filename : Mesh.hpp
 *  author   : Do Won Cha
 *  content  : Mesh class
 */

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <fstream>
#include <float.h>
#include <Eigen/Core>

#include "Triangle.hpp"

class Mesh;

struct Triangle
{
	unsigned int indices[3];
};

class Mesh
{
public:
  std::vector<Vector3>	gPositions;
  std::vector<Vector3>	gNormals;
  std::vector<Triangle>	gTriangles;

	// Transforms
	glm::vec3 pos, scale;

	// Material
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
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(2, vbo);
		glDeleteBuffers(1, &ebo);
	}

	void SetupMesh()
	{
    	glGenVertexArrays(1, &vao);
			glGenBuffers(2, vbo);
    	glGenBuffers(1, &ebo);

			glBindVertexArray(vao);
				// Bind, describe, enable position buffer
	    	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
				glBufferData(GL_ARRAY_BUFFER, gPositions.size() * sizeof(Vector3), gPositions.data(), GL_STATIC_DRAW);
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, 0);	// # of components, type, stride, offset

				// Bind normal buffer
				glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
				glBufferData(GL_ARRAY_BUFFER, gNormals.size() * sizeof(Vector3), gNormals.data(), GL_STATIC_DRAW);
				glEnableClientState(GL_NORMAL_ARRAY);
				glNormalPointer(GL_FLOAT, 0, 0);

				// Bind and describe index buffer
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, gTriangles.size() * sizeof(Triangle), gTriangles.data(), GL_STATIC_DRAW);

			glBindVertexArray(0);
	}

	void ToggleRenderMethod()
	{
		Immediate = !Immediate;
		if (Immediate)
		{
			render = &Mesh::RenderImmediate;
			printf("Rendering in immediate mode\n");
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
		}
		else
		{
			render = &Mesh::RenderBuffer;
			printf("Rendering using vertex buffer object\n");
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
			glDrawElements(GL_TRIANGLES, gTriangles.size() * 3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glPopMatrix();
	}
private:
	void PrintGLError()
	{
		GLenum err = glGetError();
		if (err != GL_NO_ERROR)
		{
			fprintf(stderr, "GL error: %s\n", glewGetErrorString(err));
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
