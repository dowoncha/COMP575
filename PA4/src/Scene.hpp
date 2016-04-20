/**
 *
 *		filename : Scene.h
 *		author   : Do Won Cha
 *		content  : Scene container to hold surfaces and lights.
 *
 */

#pragma once
#ifndef _RAY_SCENE_
#define _RAY_SCENE_

#include <list>
#include <algorithm>
#include <Eigen/Core>

#include "KDNode.hpp"
#include "Surface.h"
#include "Light.h"

/**
 *	Hit data is returned upon call to IntersectSurfaces.
 */
class HitData
{
public:
  Vector3f hitPoint, hitTime;
  Vector3f normal;
  float t, tMax;
  Surface* HitSurface;
public:
  HitData() :
    tMax(100000.0f),
    t(0),
    HitSurface(nullptr)
  { }
};

// Bounding box
struct AABB
{
	Vector3f min;
	Vector3f max;
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

class Scene
{
public:
  Scene()
  {

  }

  Scene::~Scene()
  {
      for (Surface* s : Surfaces)
      {
          delete s;
      }

      for (Light* l : Lights)
      {
          delete l;
      }
  }

  void addSurface(Surface* s)
  {
      Surfaces.push_back(s);
  }

  void addLight(Light* l)
  {
      Lights.push_back(l);
  }

  bool loadKdTree(const char* fileName, std::vector<KdNode>& kdTree)
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

  void loadMesh(std::string filename)
  {
    std::ifstream fin(filename.c_str());
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

  	printf("Loaded mesh from %s. (%lu vertices, %lu normals, %lu triangles)\n", filename.c_str(), gPositions.size(), gNormals.size(), gTriangles.size());
  	printf("Mesh bounding box is: (%0.4f, %0.4f, %0.4f) to (%0.4f, %0.4f, %0.4f)\n", xmin, ymin, zmin, xmax, ymax, zmax);
  }

  HitData Scene::IntersectSurfaces(const Ray& ray, float tMax, Surface* ignore = nullptr) const
  {
    // Intersect all surfaces using view ray
    // float tMin = tMax;
    HitData data;
    for (Surface* s : Surfaces)
    {
      if (s == ignore)
        continue;

      if (s->Intersect(ray, data.t, data.tMax, data.tPoint))
      {
          data.point      = data.tPoint;
          data.t          = data.tMax;
          data.hitSurface = s;
      }
    }

    return data;
  }

private:
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
  	return atoi(tokens.front().c_str());
  }

private:
  std::list<Surface*> surfaces;
  std::list<Light*> lights;
  std::vector<KdNode> kdTree;
};

#endif /* end of header guard for _RAY_SCENE_ */
