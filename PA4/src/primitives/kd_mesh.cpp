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
bool KdMesh::intersect(const Ray& ray, HitData& hit) const
{
	return intersect(kd_tree_.front(), ray, hit);
}

bool KdMesh::intersect(const Ray& ray) const
{
	HitData data;
	return intersect(kd_tree_.front(), ray, data);
}

// Recursively go through kd nodes
bool KdMesh::intersect(
	const KdNode& node,
	const Ray& ray,
	HitData& hit) const
{
	// Kd tree traversal
	// Start at root
	// Intersect ray with bounding box
	// If root's children has no triangle's then it is an inner node and traverse children
	// Ray must intersect current node
	if (node.boundingBox.intersect(ray)) return false;
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
		// Intersect ray with each triangle in leaf node and find the closest one
	  for (int tri_index : node.triIndex)
	  {
			const Triangle& tri = triangles_.at(tri_index);
			// If ray intersects with triangle, check the distance between ray origin
			// and triangle hit point, if closer than the current distance,
			// set hit surface, point, distance, and normal
			float tri_time;
	    if (tri.intersect(ray, tri_time))
	    {
				bTriHit = true;
				//float distance = (hit_point - ray.position()).norm();
	      if (tri_time < hit.t)
	      {
					hit.t = tri_time;
					hit.point = ray.evaluate(t);
					hit.normal = tri.normal();
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

void KdMesh::postOrder(const KdNode& node, int indent=0)
{
	if(!node.isLeaf) {
			postorder(kd_tree_.at(node.leftChildId), indent + 2);
			postorder(kd_tree_.at(node.rightChildId), indent + 2);
			if (indent) {
					std::cout << std::setw(indent) << ' ';
			}
			std::cout<< node.nodeId << "\n ";
	}
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

			triangles_.emplace_back(a - 1,b - 1,c - 1);
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
