#include "Scene.h";

Scene::Scene() :
    gNumVertices(0),
    gNumTriangles(0),
    gIndexBuffer(nullptr)
{

}

Scene::~Scene()
{

}

void Scene::LoadSphere()
{
    int width   = 32;
    int height  = 16;

    float theta, phi;
    int t;

    gNumVertices    = (height - 2) * width + 2;
    gNumTriangles   = (height - 2) * (width - 1) * 2;

    // TODO: Allocate an array for gNumVertices vertices.

    gIndexBuffer    = new int[ 3 * gNumTriangles];

    t = 0;
    for (int j = 1; j < height-1; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            theta = (float) j / (height-1) * M_PI;
            phi   = (float) i / (width-1)  * M_PI * 2;

            float   x   = sinf(theta) * cosf(phi);
            float   y   = cosf(theta);
            float   z   = -sinf(theta) * sinf(phi);

            // TODO: Set vertex t in the vertex array to {x, y, z}.

            t++;
        }
    }

    // TODO: Set vertex t in the vertex array to {0, 1, 0}.

    t++;

    // TODO: Set vertex t in the vertex array to {0, -1, 0}.

    t++;

    t = 0;
    for (int j = 0; j < height-3; ++j)
    {
        for (int i = 0; i < width-1; ++i)
        {
            gIndexBuffer[t++] = j*width + i;
            gIndexBuffer[t++] = (j+1)*width + (i+1);
            gIndexBuffer[t++] = j*width + (i+1);
            gIndexBuffer[t++] = j*width + i;
            gIndexBuffer[t++] = (j+1)*width + i;
            gIndexBuffer[t++] = (j+1)*width + (i+1);
        }
    }
    for (int i = 0; i < width-1; ++i)
    {
        gIndexBuffer[t++] = (height-2)*width;
        gIndexBuffer[t++] = i;
        gIndexBuffer[t++] = i + 1;
        gIndexBuffer[t++] = (height-2)*width + 1;
        gIndexBuffer[t++] = (height-3)*width + (i+1);
        gIndexBuffer[t++] = (height-3)*width + i;
    }

    // The index buffer has now been generated. Here's how to use to determine
    // the vertices of a triangle. Suppose you want to determine the vertices
    // of triangle i, with 0 <= i < gNumTriangles. Define:
    //
    // k0 = gIndexBuffer[3*i + 0]
    // k1 = gIndexBuffer[3*i + 1]
    // k2 = gIndexBuffer[3*i + 2]
    //
    // Now, the vertices of triangle i are at positions k0, k1, and k2 (in that
    // order) in the vertex array (which you should allocate yourself at line
    // 27).
    //
    // Note that this assumes 0-based indexing of arrays (as used in C/C++,
    // Java, etc.) If your language uses 1-based indexing, you will have to
    // add 1 to k0, k1, and k2.
}

void Scene::SetupModelTransform(float scale, const glm::vec3& center)
{
    // Make Scale matrix
    //glm::mat4x4 scaleM

    // Make Translation matrix

    //ModelTransform = Translate * Scale;
}

void Scene::SetupEyeTransform(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w, const glm::vec3& p)
{
    // EyeMatrix

    /*
    eyeTran.m[0][0] = u.x;
	eyeTran.m[0][1] = u.y;
	eyeTran.m[0][2] = u.z;
	eyeTran.m[1][0] = v.x;
	eyeTran.m[1][1] = v.y;
	eyeTran.m[1][2] = v.z;
	eyeTran.m[2][0] = w.x;
	eyeTran.m[2][1] = w.y;
	eyeTran.m[2][2] = w.z;
	temp = eyeTran.Mul(temp);
	eyeTran.m[0][3] = -temp.x;
	eyeTran.m[1][3] = -temp.y;
	eyeTran.m[2][3] = -temp.z;
    */

}

void Scene::SetupProjTransform(float l, float r, float b, float t, float n, float f)
{
    /*
    float[][] projContent = {
				{2*n/(r-l),          0,  (l+r)/(l-r),           0},
				{        0,  2*n/(t-b),  (b+t)/(b-t),           0},
				{        0,          0,  (n+f)/(n-f), 2*f*n/(f-n)},
				{        0,          0,            1,           0}
		};
		projTran = new Matrix4f(projContent);
        */
}

void Scene::SetupViewportTransform(int nx, int ny)
{

}
