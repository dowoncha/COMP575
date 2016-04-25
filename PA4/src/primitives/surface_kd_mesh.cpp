#include "surface_kd_mesh.h"

KdMesh::KdMesh(std::string meshfile, std::string kdfile, std::string material_name) :
	Surface(material_name)
{
  // Load the mesh and the kd tree from file
  load_mesh(meshfile);
  load_kd_tree(kdfile);
}

KdMesh::~KdMesh()
{}

bool KdMesh::Intersect(const Ray& ray, HitData& hit) const
{
	//Intersect(kd_tree_.at(0), ray, );
}

bool KdMesh::Intersect(const KdNode& node, const Ray& ray, float t, float& tmin) const
{
	/*
  // If the node's bounding box is hit
	if (node->boundingBox.Intersect(ray))
	{
    Vector3f normal;
    bool bTriHit = false;
    Vector3f hit_point, local_hit_point;

    const KdNode& left = kd_tree_.at(node.leftChildId);
    const KdNode& right = kd_tree_.at(node.rightChildId);

    // Recursive case: if children still have triangles go deeper
    if (left.triangles_.size() > 0 || right.triangles_.size() > 0)
    {
      bool hitLeft = Intersect(left, ray, t, tmin);
      bool hitRight = Intersect(right, ray, t ,tmin);
      return hitLeft || hitRight;
    }
    else  // Leaf of kd-tree reached
    {
      // For each triangle intersect
      for (const Triangle& tri: node.triangles)
      {
        if (tri.Intersect(ray, t, tmin))
        {
          bTriHit = true;
          tmin = t;
        }
      }

      if (bTriHit)
      {

      }
    }
	}

	*/
}

// Load the mesh from the filename
void KdMesh::load_mesh(const std::string& filename)
{
	std::ifstream fin(filename);
	if (!fin.is_open())
	{
		printf("ERROR: Unable to load mesh from %s!\n", filename);
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

			gPositions.emplace_back(x, y, z);
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

bool KdMesh::load_kd_tree(const std::string& filename)
{
	FILE* fp;
	char temp[256];
	fp = fopen(filename, "r+");
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

static void KdMesh::tokenize(char* string, std::vector<std::string>& tokens, const char* delimiter)
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
