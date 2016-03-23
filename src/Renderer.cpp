#include "Renderer.h"

using namespace Rasterizer;

Renderer::Renderer(const Scene& _scene) :
	scene(_scene),
	ScreenWidth(512),
	ScreenHeight(512),
	mat(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f))
{
}

Renderer::~Renderer()
{
    // Destroy GLFW window and context
}

void Renderer::Initialize(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (std::strcmp(argv[i], "--width") == 0)
        {
            ScreenWidth = atoi(argv[++i]);
        }
        else if (std::strcmp(argv[i], "--height") == 0)
        {
            ScreenHeight = atoi(argv[++i]);
        }
    }

    BufferInit();
}

void Renderer::BufferInit()
{
    bufferSize = ScreenWidth * ScreenHeight;

    LOG(INFO) << "Buffer size is: " << bufferSize;

    ColorBuffer.reserve(bufferSize);
    DepthBuffer.reserve(bufferSize);

	for (size_t i = 0; i < bufferSize; ++i)
	{
		ColorBuffer.push_back(glm::vec3(0.0f));
		DepthBuffer.push_back(-10000.0f);
	}
}

//DONE
void Renderer::RenderUnshaded()
{
	ClearColorBuffer();
	ClearDepthBuffer();

    for (int i = 0; i < scene.gNumTriangles; ++i)
    {
  		int base = 3 * i;

  		int k0 = scene.vIndexBuffer.at(base);
  		int k1 = scene.vIndexBuffer.at(base + 1);
  		int k2 = scene.vIndexBuffer.at(base + 2);

      // Vector should already be homogenous with x, y, z, 1.0f
  		glm::vec4 v0 = scene.ModelViewProjection() * scene.vertices.at(k0);
  		glm::vec4 v1 = scene.ModelViewProjection() * scene.vertices.at(k1);
  		glm::vec4 v2 = scene.ModelViewProjection() * scene.vertices.at(k2);

			scene.NormalizeW(v0);
			scene.NormalizeW(v1);
			scene.NormalizeW(v2);

		DrawTriangle(
			glm::vec2(v0),
			glm::vec2(v1),
			glm::vec2(v2)
			);

		/*
		ColorBuffer.at((int)v0.y * ScreenWidth + (int)v0.x) = glm::vec3(1.0f);
		ColorBuffer.at((int)v1.y * ScreenWidth + (int)v1.x) = glm::vec3(1.0f);
		ColorBuffer.at((int)v2.y * ScreenWidth + (int)v2.x) = glm::vec3(1.0f);
		*/
    }
}

void Renderer::RenderFlat()
{
	ClearColorBuffer();
	ClearDepthBuffer();

	for (int i = 0; i < scene.gNumTriangles; ++i)
	{
		int base = 3 * i;

		int k0 = scene.vIndexBuffer.at(base);
		int k1 = scene.vIndexBuffer.at(base + 1);
		int k2 = scene.vIndexBuffer.at(base + 2);

		// Vector should already be homogenous with x, y, z, 1.0f
		Vertex v0(scene.vVertices.at(k0));
		Vertex v1(scene.vVertices.at(k1));
		Vertex v2(scene.vVertices.at(k2));

		// Object to Eye space
		v0.Transform(scene.ModelView());
		v1.Transform(scene.ModelView());
		v2.Transform(scene.ModelView());

		// Calculate the color for the triangle? Still not sure what flat shading is
		glm::vec4 color = glm::vec4(CalculateFlatShading(v0, v1, v2), 1.0f);
		LOG(INFO) << "Flat shading color: " << glm::to_string(color);

		// Set the color of each vertex to Flat shaded color
		v0.color = color;
		v1.color = color;
		v2.color = color;

		// Eye to screen space
		v0.Transform(scene.ProjViewport());
		v1.Transform(scene.ProjViewport());
		v2.Transform(scene.ProjViewport());

		scene.NormalizeW(v0.pos);
		scene.NormalizeW(v1.pos);
		scene.NormalizeW(v2.pos);
		scene.NormalizeW(v0.normal);
		scene.NormalizeW(v1.normal);
		scene.NormalizeW(v2.normal);

		//DrawTriangle(glm::vec2(v0.pos), glm::vec2(v1.pos), glm::vec2(v2.pos));
		DrawTriangleFlat(v0, v1, v2);
	}
}

// Calculate normal for each vertex
// Interpolate across the vertices for each fragment
void Renderer::RenderGouraud()
{
	ClearColorBuffer();
	ClearDepthBuffer();

	for (int i = 0; i < scene.gNumTriangles; ++i)
	{
		int base = 3 * i;

		int k0 = scene.vIndexBuffer.at(base);
		int k1 = scene.vIndexBuffer.at(base + 1);
		int k2 = scene.vIndexBuffer.at(base + 2);

		Vertex v0 = scene.vVertices.at(k0);
		Vertex v1 = scene.vVertices.at(k1);
		Vertex v2 = scene.vVertices.at(k2);

		// Convert to eye space
		v0.Transform(scene.ModelView());
		v1.Transform(scene.ModelView());
		v2.Transform(scene.ModelView());

		CalculateGouraudShading(v0);
		CalculateGouraudShading(v1);
		CalculateGouraudShading(v2);

		v0.Transform(scene.ProjViewport());
		v1.Transform(scene.ProjViewport());
		v2.Transform(scene.ProjViewport());

		scene.NormalizeW(v0.pos);
		scene.NormalizeW(v1.pos);
		scene.NormalizeW(v2.pos);
		scene.NormalizeW(v0.normal);
		scene.NormalizeW(v1.normal);
		scene.NormalizeW(v2.normal);

		// IN drawing interpolate across fragment between the 3 vertices.
		DrawTriangle(v0, v1, v2);
	}
}

// Calculate lighting across polygon and cacluate color for each fragment
void Renderer::RenderPhong()
{
	ClearColorBuffer();
	ClearDepthBuffer();
}

void Renderer::DrawTriangle(const glm::vec2& v0, const glm::vec2& v1, const glm::vec2& v2)
{
  Barycentric bary(
    glm::vec4(v0, 0.0f, 1.0f),
    glm::vec4(v1, 0.0f, 1.0f),
    glm::vec4(v2, 0.0f, 1.0f)
  );

  glm::vec2 point;
  for(point.y = bary.yMin; point.y <= bary.yMax; ++point.y)
  {
    for (point.x = bary.xMin; point.x <= bary.xMax; ++point.x)
    {
      //Determine bary coordinates
      float w0 = Orient2D(v1, v2, point);
      float w1 = Orient2D(v2, v0, point);
      float w2 = Orient2D(v0, v1, point);

      if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f)
      {
        if (point.x < 0 || point.x > 512 || point.y < 0 || point.y > 512)
        {
          LOG(ERROR) << "Point out of bounds: " << point.x << ", " << point.y;
          continue;
        }

        ColorBuffer.at((int)point.x + (int)point.y * ScreenWidth) = glm::vec3(1.0f);
      }
    }
  }
}

void Renderer::DrawTriangleFlat(const Vertex& a, const Vertex& b, const Vertex& c)
{
	LOG(INFO) << "Drawing Triangle flat";

	Barycentric bary(
		a.pos,
		b.pos,
		c.pos
	);

	glm::vec2 point;
	for (point.y = bary.yMin; point.y <= bary.yMax; ++point.y)
	{
		for (point.x = bary.xMin; point.x <= bary.xMax; ++point.x)
		{
			//Determine bary coordinates
			float w0 = Orient2D(glm::vec2(b.pos), glm::vec2(c.pos), point);
			float w1 = Orient2D(glm::vec2(c.pos), glm::vec2(a.pos), point);
			float w2 = Orient2D(glm::vec2(a.pos), glm::vec2(b.pos), point);

			// Barycentric test
			if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f)
			{
				if (point.x < 0 || point.x >= 512 || point.y < 0 || point.y >= 512)
				{
					LOG(ERROR) << "Vertex out of bounds";
					continue;
				}

				// Test Depth Buffer
				glm::vec3 centroid = GetCentroid(a, b, c);
				if (centroid.z > DepthBuffer.at((int)point.x + (int)point.y * ScreenWidth))
			  {
				  LOG(INFO) << "Drawing";

					int out = (int)point.x + (int)point.y * ScreenWidth;

					DepthBuffer.at(out) = centroid.z;
					ColorBuffer.at(out) = glm::vec3(a.color);
				}
			}
		}
	}

}

void Renderer::DrawTriangle(const Vertex& a, const Vertex& b, const Vertex& c)
{
	Barycentric bary(
		a.pos,
		b.pos,
		c.pos
	);

	glm::vec2 point;
	for (point.y = bary.yMin; point.y <= bary.yMax; ++point.y)
	{
		for (point.x = bary.xMin; point.x <= bary.xMax; ++point.x)
		{
			//Determine bary coordinates
			float w0 = Orient2D(glm::vec2(b.pos), glm::vec2(c.pos), point);
			float w1 = Orient2D(glm::vec2(c.pos), glm::vec2(a.pos), point);
			float w2 = Orient2D(glm::vec2(a.pos), glm::vec2(b.pos), point);

			// Barycentric test
			if (w0 >= 0.0f && w1 >= 0.0f && w2 >= 0.0f)
			{
				if (point.x < 0 || point.x >= 512 || point.y < 0 || point.y >= 512)
				{
					LOG(ERROR) << "Vertex out of bounds";
					continue;
				}

				// Test Depth Buffer
				glm::vec3 centroid = GetCentroid(a, b, c);
				if (centroid.z > DepthBuffer.at((int)point.x + (int)point.y * ScreenWidth))
			  {
					int out = (int)point.x + (int)point.y * ScreenWidth;

					DepthBuffer.at(out) = centroid.z;
					ColorBuffer.at(out) = TriWeightAverage(w0, w1, w2, glm::vec3(a.color), glm::vec3(b.color), glm::vec3(c.color));
				}
			}
		}
	}
}

void Renderer::CalculateFlatShading(Vertex& a, Vertex& b, Vertex& c) const
{
	glm::vec3 normal = GetNormal(a, b, c);

	// Set ambient color, scale by ambient intensity of 0.2
	glm::vec3 outcolor = mat.ambient * 0.2f;
	// Calculate Diffuse
	// Get direction (normalize) from triangle centroid to the light
	// Get nl = cos a between triangle normal and dir to line
	// Scale material diffuse by the light intensity and cos a
	glm::vec3 pointToLight = glm::normalize(scene.light.pos - GetCentroid(a, b, c));
	float nl = glm::dot(normal, pointToLight);
	if (nl > 0)
	{
		outcolor += mat.diffuse * scene.light.intensity * nl;
	}

	// Calculate Specular
	// Get direction from eye to the centroid (I just negate the centroid here because the eye view is at (0, 0, 0)
	// AKA Eye(0, 0, 0) - Centroid
	// Get directoni from eye to the light
	// find angle do the cos same as above, also spec power
	glm::vec3 pointToEye = glm::normalize(-GetCentroid(a, b, c));
	glm::vec3 h = glm::normalize(pointToEye + pointToLight);
	float nh = glm::dot(normal, h);
	if (nh > 0)
	{
		outcolor += mat.specular * scene.light.intensity * std::pow(nh, scene.light.specPower);
	}

	return outcolor;
}

// Vertex coming in shulld be in eye coordinates.
void Renderer::CalculateGouraudShading(Vertex& a) const
{
	glm::vec3 out(0.0f);

	out += mat.ambient * 0.2f;

	glm::vec3 pointToLight = scene.light.pos - glm::vec3(a.pos);
	pointToLight = glm::normalize(pointToLight);
	float nl = glm::dot(glm::vec3(a.normal), pointToLight);
	if (nl > 0)
		out += mat.diffuse * (scene.light.intensity * nl);

	glm::vec3 h = glm::normalize(pointToLight - glm::vec3(a.pos));
	float nh = glm::dot(glm::vec3(a.normal), h);
	if (nh > 0)
		out += mat.specular * (scene.light.intensity * std::pow(nh, scene.light.specPower));

	a.color = glm::vec4(out, 1.0f);
}

glm::vec3 Renderer::GetNormal(const Vertex& a, const Vertex& b, const Vertex& c)
{
	auto norm = glm::normalize(glm::cross(glm::vec3(c.pos - a.pos), glm::vec3(b.pos - a.pos)));
	if (norm.z < 0)
		norm = -norm;
	return norm;
}

glm::vec3 Renderer::GetCentroid(const Vertex& a, const Vertex& b, const Vertex& c)
{
	glm::vec3 centroid;

	centroid.x = (a.pos.x + b.pos.x + c.pos.x) / 3.0f;
	centroid.y = (a.pos.y + b.pos.y + c.pos.y) / 3.0f;
	centroid.z = (a.pos.z + b.pos.z + c.pos.z) / 3.0f;

	return centroid;
}

glm::vec3 Renderer::TriWeightAverage(float a, float b, float c,
															 const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& c2) const
{
	glm::vec3 out;

	out.x = a * c0.x + b * c1.x + c * c2.x;
	out.y = a * c0.y + b * c1.y + c * c2.y;
	out.z = a * c0.z + b * c1.z + c * c2.z;

	return out;
}

float Renderer::Orient2D(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c)
{
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

void Renderer::DrawRow(int x1, int x2, int y)
{
  int rowOffset = y * ScreenWidth;
  for (int i = x1; i < x2; ++i)
  {
    //FrameBuffer[i][scanlineY] = FragmentStage(glm::ivec3(255, 255, 255));
    ColorBuffer.at(rowOffset + i) = glm::vec3(1.0f);
  }
}

void Renderer::DrawPixel(int x, int y, const glm::vec3& color)
{
	if (x < 0 || x > 512 || y < 0 || y > 512) return;

    ColorBuffer.at(y * ScreenWidth + x) = color;
}

void Renderer::ClearColorBuffer()
{
  for (auto& c : ColorBuffer)
  {
    c = glm::vec3(0.0f);
  }
}

void Renderer::ClearDepthBuffer()
{
  for (auto& d : DepthBuffer)
  {
    d = -10000.0f;
  }
}

void Renderer::OutputToPPM(const std::string& filename) const
{
    if (ColorBuffer.empty())
    {
        LOG(ERROR) << "Image Buffer empty, please call SetBuffer before outputting. size: " << ColorBuffer.size();
        return;
    }
    if (ColorBuffer.size() != bufferSize)
    {
        LOG(WARNING) << "Buffer size is not equal to image width and height";
        return;
    }

    LOG(INFO) << "Outputting to ppm file: " << filename;

    // Open stream and write ppm headers, color bit's set to 255 per channel
    //std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    std::ofstream ofs(filename, std::ios::out);

    // write headers
    ofs << "P6"			  << '\n'
        << ScreenWidth    << ' '
        << ScreenHeight   << '\n'
        << 255			  << '\n';

    for (auto color : ColorBuffer)
    {
        glm::vec3 e = GammaEncode(color);
        Pixel p = ColorToPixel(e);

        ofs << Pixel_R(p)
            << Pixel_G(p)
            << Pixel_B(p);
    }

    LOG(INFO) << "Finished outputting to " << filename << ", closing file.";
}

glm::vec3 Renderer::GammaEncode(const glm::vec3& color) const
{
    //float gamma = 1.0f / 2.4f;
    float gamma = 1.0f / 2.2f;

    glm::vec3 out;

    //out.x = (color.x <= 0.0031308f ) ? 12.92f * color.x : 1.055f * std::pow(color.x, gamma) - 0.055f;
    //out.y = (color.y <= 0.0031308f ) ? 12.92f * color.y : 1.055f * std::pow(color.y, gamma) - 0.055f;
    //out.z = (color.z <= 0.0031308f ) ? 12.92f * color.z : 1.055f * std::pow(color.z, gamma) - 0.055f;
    out.x = std::pow(color.x, gamma);
		out.y = std::pow(color.y, gamma);
		out.z = std::pow(color.z, gamma);

    return out;
}

glm::vec3 Renderer::PinToUnit(const glm::vec3& color) const
{
  float x = std::max(0.0f, std::min(1.0f, color.x));
  float y = std::max(0.0f, std::min(1.0f, color.y));
  float z = std::max(0.0f, std::min(1.0f, color.z));

  return glm::vec3(x, y, z);
}

Pixel Renderer::ColorToPixel(const glm::vec3& color) const
{
    glm::vec3 pinned = PinToUnit(color);

    uint8_t uR = (uint8_t) (pinned.x * 255.9999f);
    uint8_t uG = (uint8_t) (pinned.y * 255.9999f);
    uint8_t uB = (uint8_t) (pinned.z * 255.9999f);

    return MakeRGBA(uR, uG, uB, 255);
}
