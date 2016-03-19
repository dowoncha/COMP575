#include "Scene.h"

using namespace Rasterizer;

Scene::Scene() :
    gNumVertices(0),
    gNumTriangles(0),
    gIndexBuffer(nullptr),
    ViewTransform(1.0f)
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

    gNumVertices    = (height - 2) * width + 2;
    gNumTriangles   = (height - 2) * (width - 1) * 2;

    // RESOLVED: Allocate an array for gNumVertices vertices.
    // Using a vector, no allocation needed

    gIndexBuffer    = new int[ 3 * gNumTriangles];

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
            vertices.emplace_back(x, y, z);
        }
    }

    // TODO: Set vertex t in the vertex array to {0, 1, 0}.
    vertices.emplace_back(0.0f, 1.0f, 0.0f);

    // TODO: Set vertex t in the vertex array to {0, -1, 0}.
    vertices.emplace_back(0.0f, -1.0f, 0.0f);

    int t = 0;
    for (int j = 0; j < height-3; ++j)
    {
        for (int i = 0; i < width-1; ++i)
        {
            vIndexBuffer.push_back(j * width + i);
            vIndexBuffer.push_back((j+1) * width + (i + 1));
            vIndexBuffer.push_back(j * width + (i + 1));
            vIndexBuffer.push_back(j * width + i);
            vIndexBuffer.push_back((j + 1) * width + i);
            vIndexBuffer.push_back((j + 1) * width + (i + 1));
        }
    }
    for (int i = 0; i < width-1; ++i)
    {
        vIndexBuffer.push_back((height - 2) * width);
        vIndexBuffer.push_back(i);
        vIndexBuffer.push_back(i + 1);
        vIndexBuffer.push_back((height-2) * width + 1);
        vIndexBuffer.push_back((height-3) * width + (i + 1));
        vIndexBuffer.push_back((height - 3) * width + i);
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
    //
    printf("Sphere loaded, # of vertices: %d, # of Triangles: %d\n", gNumVertices, gNumTriangles);
}

void Scene::SetupModelTransform(float scale, const glm::vec3& center)
{
    // Make Scale matrix
    glm::mat4x4 scale = glm::scale(glm::mat4x4(1.0f), glm::vec3(scale));

    // Make Translation matrix
    glm::mat4x4 translate = glm::translate(glm::mat4x4(1.0f), center);

    ModelTransform = translate * scale;
}

void Scene::SetupViewTransform(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w, const glm::vec3& p)
{
    ViewTransform[0] = glm::vec4(u, 0);
    ViewTransform[1] = glm::vec4(v, 0);
    ViewTransform[2] = glm::vec4(w, 0);

    // Homogenous coordinate magic
    glm::vec4 temp = ViewTransform * glm::vec4(p, 1.0f);
    ViewTransform[0][3] = -temp.x;
    ViewTransform[1][3] = -temp.y;
    ViewTransform[2][3] = -temp.z;
}

void Scene::SetupProjTransform(float l, float r, float b, float t, float n, float f)
{
  float n2 = 2 * n;

  ProjTransform[0] = glm::vec4( n2 / (r - 1),    0, (l+r)/(l-r), 0);
  ProjTransform[1] = glm::vec4( 0, n2 / (r - 1), 0, (b+t)/(b-t));
  ProjTransform[2] = glm::vec4( 0, 0, (n+f)/(n-f), n2 * f / (f - n));
  ProjTransform[3] = glm::vec4( 0, 0, 1, 0);
}

void Scene::SetupViewportTransform(int nx, int ny)
{
    ViewportTransform[0] = glm::vec4(nx / 2, 0, 0, (nx - 1) / 2);
    ViewportTransform[1] = glm::vec4(0, ny / 2, 0, (ny - 1) / 2);
    ViewportTransform[2] = glm::vec4(0, 0, 1, 0);
    ViewportTransform[3] = glm::vec4(0, 0, 0, 1);
}

void Scene::SetupMVP()
{
    // I hope this is the right order
    MVP = ViewportTransform * ProjTransform * ViewTransform * ModelTransform;
}

glm::mat4x4 Scene::ModelViewProj() const
{
    return MVP;
}

glm::mat4x4 Scene::ModelView() const
{
  return ViewTransform * ModelTransform;
}
