#include "Scene.h"

using namespace Rasterizer;

Scene::Scene() :
    gNumVertices(0),
    gNumTriangles(0),
    ViewTransform(1.0f),
    light(glm::vec3(-4.0f, -4.0f, -3.0f), 1.0f)
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

    for (int j = 1; j < height-1; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            theta = (float) j / (height-1) * M_PI;
            phi   = (float) i / (width-1)  * M_PI * 2.0f;

            float   x   = sinf(theta) * cosf(phi);
            float   y   = cosf(theta);
            float   z   = -sinf(theta) * sinf(phi);

			      glm::vec4 vert(x, y, z, 1.0f);

            // TODO: Set vertex t in the vertex array to {x, y, z}.
            vertices.push_back(vert);

			// New shit
			glm::vec4 normal = glm::normalize(vert);		// The normal of a sphere is from center to the vertex negated.
			Vertex v(
				glm::vec4(x, y, z, 1.0f),
				glm::vec4(0.0f),
				normal
			);

			vVertices.push_back(v);
        }
    }

    // TODO: Set vertex t in the vertex array to {0, 1, 0}.
    vertices.emplace_back(0.0f, 1.0f, 0.0f, 1.0f);
	vVertices.emplace_back(
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f),
		glm::vec4(0.0f, -1.0f, 0.0f, 1.0f)
		);

    // TODO: Set vertex t in the vertex array to {0, -1, 0}.
    vertices.emplace_back(0.0f, -1.0f, 0.0f, 1.0f);
	vVertices.emplace_back(
		glm::vec4(0.0f, -1.0f, 0.0f, 1.0f),
		glm::vec4(0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
		);

    assert(vertices.size() == gNumVertices);
	assert(vVertices.size() == gNumVertices);

    int t = 0;
    for (int j = 0; j < height - 3; ++j)
    {
        for (int i = 0; i < width - 1; ++i)
        {
            vIndexBuffer.push_back(j * width + i);
            vIndexBuffer.push_back((j + 1) * width + (i + 1));
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

    //assert(vIndexBuffer.size() == gNumTriangles / 3);

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
    printf("Vertex Buffer Size: %d, IndexBuffer Size: %d\n", vertices.size(), vIndexBuffer.size());
}

void Scene::SetupModelTransform(const glm::vec3& scale, const glm::vec3& center)
{
    // Make Scale matrix
    glm::mat4x4 scaleMat(
      glm::vec4(scale.x, 0.0f, 0.0f, 0.0f),
      glm::vec4(0.0f, scale.y, 0.0f, 0.0f),
      glm::vec4(0.0f, 0.0f, scale.z, 0.0f),
      glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    // Make Translation matrix
    glm::mat4x4 transMat(
      glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
      glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
      glm::vec4(0.0f, 0.0f, 1.0f, -7.0f),
      glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
    );

    ModelTransform = scaleMat * transMat;
    //ModelTransform = transMat * scaleMat;

    //ModelTransform = glm::transpose(ModelTransform);

    LOG(INFO) << "Model Transform: " << glm::to_string(ModelTransform);
}

void Scene::SetupViewTransform(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w, const glm::vec3& p)
{
    ViewTransform[0] = glm::vec4(u, 0.0f);
    ViewTransform[1] = glm::vec4(v, 0.0f);
    ViewTransform[2] = glm::vec4(w, 0.0f);

    // Homogenous coordinate magic
    glm::vec4 temp = ViewTransform * glm::vec4(p, 1.0f);
    ViewTransform[0][3] = -temp.x;
    ViewTransform[1][3] = -temp.y;
    ViewTransform[2][3] = -temp.z;

    //ViewTransform = glm::transpose(ViewTransform);

    LOG(INFO) << "View Transform: " << glm::to_string(ViewTransform);
}

void Scene::SetupProjTransform(float l, float r, float b, float t, float n, float f)
{
  ProjTransform[0] = glm::vec4( 2.0f*n/(r-l), 0.0f, (l+r)/(l-r), 0.0f);
  ProjTransform[1] = glm::vec4( 0.0f, 2.0f*n/(t-b), (b+t)/(b-t), 0.0f);
  ProjTransform[2] = glm::vec4( 0.0f, 0.0f, (n+f)/(n-f), 2.0f*n*f/(f-n));
  ProjTransform[3] = glm::vec4( 0.0f, 0.0f, 1.0f, 0.0f);

  //ProjTransform = glm::transpose(ProjTransform);

  LOG(INFO) << "Projection Transform: " << glm::to_string(ProjTransform);
}

void Scene::SetupViewportTransform(int nx, int ny)
{
    ViewportTransform[0] = glm::vec4(nx / 2.0f, 0.0f, 0.0f, (nx - 1) / 2.0f);
    ViewportTransform[1] = glm::vec4(0.0f, ny / 2.0f, 0.0f, (ny - 1) / 2.0f);
    ViewportTransform[2] = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    ViewportTransform[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    LOG(INFO) << "Viewport Transform: " << glm::to_string(ViewportTransform);
}

void Scene::SetupMVP()
{
    MVP = ModelTransform * ViewTransform * ProjTransform * ViewportTransform;
    MVP = glm::transpose(MVP);
    LOG(INFO) << "MVP: " << glm::to_string(MVP);
	  LOG(INFO) << "MV * PV: " << glm::to_string(ProjViewport() * ModelView());
}

glm::mat4x4 Scene::ModelView() const
{
	return glm::transpose(ModelTransform * ViewTransform);
}

glm::mat4x4 Scene::ProjViewport() const
{
	return glm::transpose(ProjTransform * ViewportTransform);
}

glm::mat4x4 Scene::ModelViewProjection() const
{
  return MVP;
}

void Scene::NormalizeW(glm::vec4& v) const
{
  v.x /= v.w;
  v.y /= v.w;
  v.z /= v.w;
  v.w = 1.0f;
}

//MVP
//-512, 0, 511, -1788.5
//0, -512, 511, -1788.5
//0, 0, -2.000339, 6.81
//0, 0, 2, -7
