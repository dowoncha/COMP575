/**
 *  filename : Mesh.hpp
 *  author   : Do Won Cha
 *  content  : Mesh class
 */

#pragma once
#ifndef _RAY_MESH_
#define _RAY_MESH_

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <fstream>
#include <float.h>
#include <Eigen/Core>

using namespace Eigen;


class Triangle : public Surface
{
public:
  typedef Matrix<unsigned int, 3, 1> Vector3u;

  Triangle();

  ~Triangle();

  bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const override;
  bool Intersect(const Ray& ray, float tMax, float& t) const override;
  bool Intersect(const Ray& ray, float tMax) const override;

  Vector3f GetNormal() const;
  Vector3f GetNormal(const Vector3f& p) const override;
private:
  Vector3u indices;
};

class KdMesh: public Surface
{
private:
	// Bounding box
	struct AABB
	{
		Vector3f min;
		Vector3f max;

		bool hit(const Ray& ray)
		{
			float tx1 = (min(0) - ray.pos(0)) * ray.direction.inverse();
			float tx2 = (max(0) - ray.pos(0)) * ray.direction.inverse();

			double tmin = min(tx1, tx2);
	    double tmax = max(tx1, tx2);

	    double ty1 = (b.min.y - r.x0.y)*r.n_inv.y;
	    double ty2 = (b.max.y - r.x0.y)*r.n_inv.y;

	    tmin = max(tmin, min(ty1, ty2));
	    tmax = min(tmax, max(ty1, ty2));

	    return tmax >= tmin;
		}
	};

	// kd-tree nodes
	struct KdNode
	{
		int nodeId;
		AABB boundingBox;
		int leftChildId;
		int rightChildId;
		int splitAxis;
		float splitPosition;
		bool isLeaf;
		std::vector<int> triIndex;
	};

	std::vector<KdNode>	 kdTree;
	std::vector<Vector3>	gPositions;
  std::vector<Vector3>	gNormals;
  std::vector<Triangle>	gTriangles;

	KdMesh(const Material& mat) :
		surface(mat)
	{
	}

	~KdMesh()
	{

	}

	bool hit(KdNode* node, const Ray& ray, float t, float& tmin) const {
		if (node->boundingBox.hit(ray))
		{

		}
	}

  bool Intersect(const Ray& ray, float tMax, float& t) const override;
  bool Intersect(const Ray& ray, float tMax) const override;

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


	bool loadKdTree(const char* fileName)
	{
		FILE* fp;
		char temp[256];
		fp = fopen(fileName, "r+");
		if (!fp)
			return false;
		int nodeId = -1;
		while (true)
		{
			nodeId++;
			*temp = NULL;
			fscanf(fp, "%s{ ", temp);
			if (strcmp(temp, "inner{") == 0)
			{
				KdNode kd;
				kd.nodeId = nodeId;
				kd.isLeaf = false;
				fscanf(fp, "%f %f %f %f %f %f ; %d %d %d %f }", &kd.boundingBox.min.x, &kd.boundingBox.min.y, &kd.boundingBox.min.z, &kd.boundingBox.max.x, &kd.boundingBox.max.y, &kd.boundingBox.max.z, &kd.leftChildId, &kd.rightChildId, &kd.splitAxis, &kd.splitPosition);
				kdTree.push_back(kd);
			}
			else if (strcmp(temp, "leaf{") == 0)
			{
				KdNode kd;
				kd.nodeId = nodeId;
				kd.isLeaf = true;
				fscanf(fp, "%f %f %f %f %f %f ;", &kd.boundingBox.min.x, &kd.boundingBox.min.y, &kd.boundingBox.min.z, &kd.boundingBox.max.x, &kd.boundingBox.max.y, &kd.boundingBox.max.z);
				char token[256];
				while (true)
				{
					fscanf(fp, " %s", token);
					if (strcmp(token, "}") == 0)
						break;
					int triIndex = atoi(token);
					kd.triIndex.push_back(triIndex);
				}
				kdTree.push_back(kd);
			}
			else
				break;
		}
		printf("Kd file parsed succesfully.\n");
		return true;
	}

private:
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

#endif // _RAY_MESH_
