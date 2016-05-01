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
#include <iostream>
#include <iomanip>
#include <fstream>
#include <float.h>

#include "material.h"
#include "surface.h"

namespace raytracer
{

using namespace Eigen;

class Triangle
{
  typedef Matrix<unsigned int, 3, 1> triangle_t;
public:
  Triangle(unsigned int a, unsigned int b, unsigned int c) :
    indices(a, b, c)
  {}

  Triangle(const triangle_t& t_indices) :
    indices(t_indices)
  {}

  bool intersect(const Ray& ray, HitData& hit) const
  {
    const float EPSILON = 1e-6;

  	//Find vectors for two edges sharing V1
  	Vector3f edge1 = indices_(1) - indices_(0);
  	Vector3f edge2 = indices_(2) - indices_(0);

  	// Calcluate cross product between ray direction and edge2
  	// The find determinant
  	Vector3f ray_cross_edge = ray.direction().cross(edge2);
  	float det = edge1.dot(ray_cross_edge);

  	// ray and triangle are parallel if det is close to 0, no backface culling
   	if(det < EPSILON) return false;

   	float inv_det = 1.0f / det;

  	Vector3f v0toray = ray.position() - v0;

  	float u = v0toray.dot(ray_cross_edge) * inv_det;

  	//The intersection lies outside of the triangle
  	if(u < 0.0f || u > 1.0f) return false;

   	//Prepare to test v parameter
   	Vector3f qvec = v0toray.cross(edge1);

  	float v = ray.direction().dot(qvec) * inv_det;

   	//The intersection lies outside of the triangle
   	if(v < 0.0f || u + v  > 1.0f) return false;

   	float calctime = edge2.dot(qvec) * inv_det;

  	// If the triangle hit time is greater than the epsilon
   	if(calctime > EPSILON)
  	{
      if (calctime < hit.t)
      {
        hit.t = calctime;
        hit.point = ray.evaluate(hit.t);
        hit.normal = normal();
      }
  	 	return true;
   }

   // No hit, no win
   return false;
  }

  Vector3f normal() const
  {
    // Get vertices of the trianlge
  	//Find vectors for two edges sharing V1
  	Vector3f edge1 = indices(1) - indices(0);
  	Vector3f edge2 = indices(2) - indices(0);

  	// Calcluate cross product between ray direction and edge2
  	// The find determinant
  	return edge1.cross(edge2).normalized();
  }
private:
  Matrix<unsigned int, 3, 1> indices;
}

// Bounding box
struct AABB
{
  Vector3f min;
  Vector3f max;

  // Bounding box & ray hit test
  bool Intersect(const Ray& ray) const
  {
    Vector3f inv;
    inv << 1.0f / ray.direction()(0)
        << 1.0f / ray.direction()(1)
        << 1.0f / ray.direction()(2);

    float tx1 = (min(0) - ray.position()(0)) * inv(0);
    float tx2 = (max(0) - ray.position()(0)) * inv(0);

    float tmin = std::min(tx1, tx2);
    float tmax = std::max(tx1, tx2);

    float ty1 = (min(1) - ray.position()(1)) * inv(1);
    float ty2 = (max(1) - ray.position()(1)) * inv(1);

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    float tz1 = (min(2) - ray.position()(2)) * inv(2);
    float tz2 = (max(2) - ray.position()(2)) * inv(2);

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return (tmax >= tmin && tmax > 0);
  }
};

// kd-tree nodes struct
struct KdNode
{
  KdNode() :
    leftChildId(-1),
    rightChildId(-1)
  {}

  int nodeId;
  AABB boundingBox;
  int leftChildId;
  int rightChildId;
  int splitAxis;
  float splitPosition;
  bool isLeaf;
  std::vector<int> triIndex;

  void print() const
  {
    printf("Node: %d, left: %d, right %d: isLeaf: %d, trisize: %lu\n", nodeId, leftChildId, rightChildId, isLeaf, triIndex.size());
  }
};

class KdMesh: public Surface
{
  typedef std::vector<KdNode> kd_tree_t;
  typedef std::vector<triangle_t> triangles_t;
public:
	KdMesh(std::string meshfile, std::string kdfile, material_t material);

	~KdMesh();

  // Derived function for surface intersection
	bool intersect(const Ray& ray, Vector3f& hit_point, Vector3f& hit_normal) const;
  bool intersect(const Ray& ray) const;
private:
  // Recursively intersect starting at root node, hit data contains the hit point and normal
  bool intersect(const KdNode& node, const Ray& ray, HitData& hit) const;

  // Print kdtree in post order
  void postOrder(const KdNode& node, int indent = 0);
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
