#include <stdio.h>
#include <string.h>
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include <float.h>

struct vec3
{
	float x;
	float y;
	float z;
};

struct AABB
{
	vec3 min;
	vec3 max;
};

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

bool load_kdTree(const char* fileName, std::vector<KdNode> &kdTree)
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
