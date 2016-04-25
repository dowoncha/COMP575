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

class Triangle : public Surface
{
friend class KdMesh;

public:
  // Triangle indices
  typedef Matrix<unsigned int, 3, 1> Vector3u;

  Triangle(unsigned int a, unsigned int b, unsigned int c) :
    indices_(a, b, c)
  {
  }

  Vector3u indices() { return indices_; }

  bool Intersect(const Ray& ray, HitData& data) override
  {
    // Vertices
    const Vector3f& p0 = owner.vertices_.at(indices_(0));
    const Vector3f& p1 = owner.vertices_.at(indices_(1));
    const Vector3f& p2 = owner.vertices_.at(indices_(2));

    // Calculate the edges
    Vector3f edge1 = p1 - p0;
    Vector3f edge2 = p2 - p0;

    Vector3f q = ray.distance().cross(e2);
    float alpha = edge1.dot(q);             // Determinant of matrix m

    if (alpha > eps && alpha < eps)
    {
      // data.u = 0
      // data.v = 0
      // data.time = 0
      return false;
    }
    float invAlpha = 1.0f / alpha;
    Vector3f source = ray.position() - p0;
    float u = invAlpha * source.dot(q);
    if (u < 0.0f)
    {
      // data.u = 0
      // data.v = 0
      // data.time = 0
      return false;
    }

    Vector3f r = source.cross(edge1);
    float v = invAlpha * ray.direction().dot(r);
    if (v < 0.0f)
    {
      // data.u = 0
      // data.v = 0
      // data.time = 0
      return false;
    }

    // Hit data here
    // float t = f * edge2.dot(r);
    return true;
  }

  // TODO: Calculate triangle normal here
  Vector3f normal() const override
  {
    return normal_;
  }
private:
  Vector3u indices_;
  Vector3f normal_;
};

class KdMesh: public Surface
{
  typedef std::vector<KdNode> kd_tree_t;
  typedef std::vector<Triangle> triangles_t;
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
