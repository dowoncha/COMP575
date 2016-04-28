#include "surface_kd_mesh.h"

using namespace raytracer;

KdMesh::KdMesh(std::string meshfile, std::string kdfile, material_t material) :
	Surface(material)
{
  // Load the mesh and the kd tree from file
  load_mesh(meshfile);
  load_kd_tree(kdfile);
}

KdMesh::~KdMesh()
{}

// Return the closest hit point from ray
bool KdMesh::Intersect(const Ray& ray, Vector3f& hit_point, Vector3f& hit_normal) const
{
	HitData hit;
	hit.distance = 100000.0f;
	if (Intersect(kd_tree_.front(), ray, hit))
	{
		hit_point = hit.point;
		hit_normal = hit.normal;
		return true;
	}

	return false;
}

bool KdMesh::Intersect(const Ray& ray) const
{
	HitData data;
	return Intersect(kd_tree_.front(), ray, data);
}

bool KdMesh::Intersect(
	const KdNode& node,
	const Ray& ray,
	HitData& hit) const
{
	// Kd tree traversal
	// Start at root
	// Intersect ray with bounding box
	// If root's children has no triangle's then it is an inner node and traverse children
	// Ray must intersect current node
	if (node.boundingBox.Intersect(ray))
	{
		//node.print();
		// Recurse through any children nodes if they have triangles
		//if (left.leftChildId != -1 || right.rightChildId != -1)
		if (!node.isLeaf)
		{
			const KdNode& left = kd_tree_.at(node.leftChildId);
			const KdNode& right = kd_tree_.at(node.rightChildId);

		  return Intersect(left, ray, hit) || Intersect(right, ray, hit);
		}
		else // Leaf node reached
		{
			bool bTriHit = false;
			Vector3f hit_point;
			// Intersect ray with each triangle in leaf node and find the closest one
		  for (int tri_index : node.triIndex)
		  {
				// If ray intersects with triangle, check the distance between ray origin
				// and triangle hit point, if closer than the current distance,
				// set hit surface, point, distance, and normal
		    if (intersect_tri(ray, triangles_.at(tri_index), hit_point))
		    {
					float distance = (hit_point - ray.position()).norm();
		      if (distance < hit.distance)
		      {
						bTriHit = true;
						hit.surface = dynamic_cast<Surface*>(const_cast<KdMesh*>(this));
		        hit.point = hit_point;
						hit.distance = distance;
		        hit.normal = normal_tri(triangles_.at(tri_index));
		      }
		    }
			}

			// Triangle was hit in leaf node
		  if (bTriHit)
				return true;

			// No triangles hit in leaf node
			return false;
		}
	}
	// Ray did not intersect bounding box
	return false;
}

bool KdMesh::intersect_tri(const Ray& ray, const triangle_t& triangle, Vector3f& hit_point) const
{
	const float EPSILON = 1e-6;

	// Get vertices of the trianlge
	const Vector3f& v0 = vertices_.at(triangle(0));
	const Vector3f& v1 = vertices_.at(triangle(1));
	const Vector3f& v2 = vertices_.at(triangle(2));

	//Find vectors for two edges sharing V1
	Vector3f edge1 = v1 - v0;
	Vector3f edge2 = v2 - v0;

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

 	float t = edge2.dot(qvec) * inv_det;

	// If the triangle hit time is greater than the epsilon
 	if(t > EPSILON)
	{
		hit_point = ray.evaluate(t);
	 	return true;
 }

 // No hit, no win
 return false;
}

Vector3f KdMesh::normal_tri(const triangle_t& tri) const
{
	// Get vertices of the trianlge
	const Vector3f& v0 = vertices_.at(tri(0));
	const Vector3f& v1 = vertices_.at(tri(1));
	const Vector3f& v2 = vertices_.at(tri(2));

	//Find vectors for two edges sharing V1
	Vector3f edge1 = v1 - v0;
	Vector3f edge2 = v2 - v0;

	// Calcluate cross product between ray direction and edge2
	// The find determinant
	return edge1.cross(edge2).normalized();
}

// Load the mesh from the filename
void KdMesh::load_mesh(const std::string& filename)
{
	std::ifstream fin(filename);
	if (!fin.is_open())
	{
		printf("ERROR: Unable to load mesh from %s!\n", filename.c_str());
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

			vertices_.emplace_back(x, y, z);
		}
		else if (tokens[0] == "vn")
		{
			float x = atof(tokens[1].c_str());
			float y = atof(tokens[2].c_str());
			float z = atof(tokens[3].c_str());
			Vector3f normal(x,y,z);
			normals_.push_back(normal);
		}
		else if (tokens[0] == "f")
		{
			unsigned int a = face_index(tokens[1].c_str());
			unsigned int b = face_index(tokens[2].c_str());
			unsigned int c = face_index(tokens[3].c_str());
			triangle_t triangle(a-1,b-1,c-1);
			triangles_.push_back(triangle);
		}
	}

	fin.close();

	printf("Loaded mesh from %s. (%lu vertices, %lu normals, %lu triangles)\n", filename.c_str(), vertices_.size(), normals_.size(), triangles_.size());
	printf("Mesh bounding box is: (%0.4f, %0.4f, %0.4f) to (%0.4f, %0.4f, %0.4f)\n", xmin, ymin, zmin, xmax, ymax, zmax);
}

bool KdMesh::load_kd_tree(const std::string& filename)
{
	FILE* fp;
	char temp[256];
	fp = fopen(filename.c_str(), "r+");
	if (!fp)
		return false;
	int nodeId = -1;
	int inner = 0;
	int leaf = 0;
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
			Vector3f boxMin;
			Vector3f boxMax;
			fscanf(fp, "%f %f %f %f %f %f ; %d %d %d %f }", &boxMin(0), &boxMin(1), &boxMin(2), &boxMax(0),&boxMax(1), &boxMax(2), &kd.leftChildId, &kd.rightChildId, &kd.splitAxis, &kd.splitPosition);
			kd.boundingBox.min = boxMin;
			kd.boundingBox.max = boxMax;
			kd_tree_.push_back(kd);
			inner++;
		}
		else if (strcmp(temp, "leaf{") == 0)
		{
			KdNode kd;
			kd.nodeId = nodeId;
			kd.isLeaf = true;
			Vector3f boxMin;
			Vector3f boxMax;
			fscanf(fp, "%f %f %f %f %f %f ;", &boxMin(0), &boxMin(1), &boxMin(2), &boxMax(0),&boxMax(1), &boxMax(2));
			kd.boundingBox.min = boxMin;
			kd.boundingBox.max = boxMax;
			char token[256];
			while (true)
			{
				fscanf(fp, " %s", token);
				if (strcmp(token, "}") == 0)
					break;
				int triIndex = atoi(token);
				kd.triIndex.push_back(triIndex);
			}
			kd_tree_.push_back(kd);
			leaf++;
		}
		else
			break;
	}
	printf("Kd file parsed succesfully. With %d inner nodes and %d leaves\n", inner, leaf);
	return true;
}

void KdMesh::tokenize(char* string, std::vector<std::string>& tokens, const char* delimiter)
{
	char* token = strtok(string, delimiter);
	while (token != NULL)
	{
		tokens.push_back(std::string(token));
		token = strtok(NULL, delimiter);
	}
}

int KdMesh::face_index(const char* string)
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
