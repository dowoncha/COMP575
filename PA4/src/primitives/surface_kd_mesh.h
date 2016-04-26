/**
 *  filename : Mesh.hpp
 *  author   : Do Won Cha
 *  content  : Mesh class
 */

#pragma once
#ifndef _RAY_MESH_
#define _RAY_MESH_

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <array>
#include <queue>
#include <fstream>
#include <float.h>

#include "material.h"
#include "surface.h"

namespace raytracer
{

using namespace Eigen;

// Bounding box
struct AABB
{
  Vector3f min;
  Vector3f max;

  // Bounding box & ray hit test
  bool Intersect(const Ray& ray) const
  {
    float d_inv_x = 1.0f / ray.direction()(0);
    float d_inv_y = 1.0f / ray.direction()(1);

    float tx1 = (min(0) - ray.position()(0)) * d_inv_x;
    float tx2 = (max(0) - ray.position()(0)) * d_inv_x;

    float tmin = std::min(tx1, tx2);
    float tmax = std::max(tx1, tx2);

    float ty1 = (min(1) - ray.position()(1)) * d_inv_y;
    float ty2 = (max(1) - ray.position()(1)) * d_inv_y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    return tmax >= tmin;
  }
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

class KdMesh: public Surface
{
  typedef Matrix<unsigned int, 3, 1> triangle_t;
  typedef std::vector<KdNode> kd_tree_t;
  typedef std::vector<triangle_t> triangles_t;
public:
	KdMesh(std::string meshfile, std::string kdfile, material_t material);

	~KdMesh();

	bool Intersect(const Ray& ray, Vector3f& hit_point, Vector3f& hit_normal) const;

  bool Intersect(const Ray& ray) const;

  bool Intersect(
  	const KdNode& node,
  	const Ray& ray,
  	float& t,
  	float& tmin,
  	HitData& hit) const;

  bool intersect_tri(const Ray& ray, const triangle_t& triangle, float& t) const;

  Vector3f normal_tri(triangle_t tri) const;
private:
  // Load the mesh from the filename
	void load_mesh(const std::string& filename);

  // Load kd-tree data from the file
	bool load_kd_tree(const std::string& filename);

	static void tokenize(char* string, std::vector<std::string>& tokens, const char* delimiter);

	static int face_index(const char* string);
private:
  kd_tree_t	  kd_tree_;
  triangles_t	triangles_;
  std::vector<Vector3f>	vertices_;
  std::vector<Vector3f>	normals_;
};

} // end of namespace

#endif // _RAY_MESH_
