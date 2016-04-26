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

#include "material.h"

namespace raytracer
{

using namespace Eigen;

class KdMesh: public Surface
{
  typedef std::vector<KdNode> kd_tree_t;
  typedef std::vector<Vector3u> triangles_t;
public:
	KdMesh(std::string meshfile, std::string kdfile, std::shared_ptr<Material> material);

	~KdMesh();

	bool Intersect(const Ray& ray, HitData& hit) const;

	bool Intersect(const KdNode& node, const Ray& ray, float t, float& tmin) const;
private:
  // Load the mesh from the filename
	void load_mesh(const std::string& filename);

  // Load kd-tree data from the file
	bool load_kd_tree(const std::string& filename);

	static void tokenize(char* string, std::vector<std::string>& tokens, const char* delimiter);

	static int face_index(const char* string);
private:
  // Bounding box
  struct AABB
  {
    Vector3f min;
    Vector3f max;

    // Bounding box & ray hit test
    bool hit(const Ray& ray);
  };

  // kd-tree nodes struct
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

  kt_tree_t	  kd_tree_;
  triangles_t	triangles_;
  std::vector<Vector3f>	vertices_;
  std::vector<Vector3f>	vertex_normals_;

  material_t material;
};

#endif // _RAY_MESH_
