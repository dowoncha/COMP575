/*******************************************************************************
 *
 *	filename:    Renderer.h
 *	author:      Do Won Cha
 *	content:     GL Rendering class
 *
 ******************************************************************************/

#pragma once
#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <array>
#include <cstdlib> // Needed for atoi
#include <cassert>
#include <algorithm>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "easylogging++.h"

#include "Scene.h"
#include "Utility.hpp"

#define GPIXEL_SHIFT_R  24
#define GPIXEL_SHIFT_G  16
#define GPIXEL_SHIFT_B   8
#define GPIXEL_SHIFT_A   0

namespace Rasterizer
{

/**
*  Defines our 32bit pixel to be just an int. It stores its components based
*  Temporary pixel implementation packed into an uint32_t
*/
typedef uint32_t Pixel;

///////////////////////////////////////////////////////////////////////////////

static inline int Pixel_R(Pixel p) { return (p >> GPIXEL_SHIFT_R) & 0xFF; }
static inline int Pixel_G(Pixel p) { return (p >> GPIXEL_SHIFT_G) & 0xFF; }
static inline int Pixel_B(Pixel p) { return (p >> GPIXEL_SHIFT_B) & 0xFF; }
static inline int Pixel_A(Pixel p) { return (p >> GPIXEL_SHIFT_A) & 0xFF; }

/*
*  Asserts that rgba are already in premultiply form, and simply
*  packs them into a GPixel.
*/
static inline Pixel MakeRGBA(unsigned r, unsigned g, unsigned b, unsigned a) {
	assert(a <= 255);
	assert(r <= a);
	assert(g <= a);
	assert(b <= a);

	return 	(r << GPIXEL_SHIFT_R) |
			(g << GPIXEL_SHIFT_G) |
			(b << GPIXEL_SHIFT_B) |
			(a << GPIXEL_SHIFT_A);
}

Pixel ColorToPixel(const glm::vec4& color)
{
	// TODO: Temporarily using normalize, but this is potentially incorrect
	// each x,w,y,z value needs to be pinned between 0-1, not normalized
	auto pinned = glm::normalize(color);

	float fA = pinned.a * 255.9999f;			//Convert from 0-1 to 0-255
	uint8_t uA = (uint8_t) fA;
	uint8_t uR = (uint8_t) (pinned.r * fA);		//Multiply rgb values by the new alpha
	uint8_t uG = (uint8_t) (pinned.g * fA);
	uint8_t uB = (uint8_t) (pinned.b * fA);

	return MakeRGBA(uR, uG, uB, uA);		//Returned the packed pixel
}

class Renderer
{
private:
	class Vertex
	{
	public:
		glm::vec3 pos, color;
	};

	class Triangle
	{
	public:
		std::array<Vertex, 3> vertices;
	};

public:
	/**
		*  Provide the output buffer and the width and height of the screen
		*/
	Renderer(const Scene&);

	~Renderer();

	void Initialize(int argc, char* argv[]);

	void Run();
private:
	void GLInit();

	void BufferInit();

	void Resize(int width, int height);

	void Render();

	void Rasterize(const Triangle& tri);

	void DrawTopTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	void DrawBotTriangle(const Vertex& v0, const Vertex& v1, const Vertex& v2);

	void DrawRow(int x1, int x2, int scanlineY);

	glm::vec3 FragmentStage(const glm::vec3& color);

	glm::vec3 GammaEncode(const glm::vec3& color);

	/**
	 * Clear the entire color and depth buffers
	 */
	void ClearColorBuffer();
	void ClearDepthBuffer();
private:
	const Scene& scene;							// Should this be a reference?

	int ScreenWidth, ScreenHeight;

	// Buffers
	uint32_t FrameBuffer;
	std::vector<glm::vec3> ColorBuffer;
	std::vector<float> DepthBuffer;

	size_t bufferSize;
	GLFWwindow* window;
};

} //end of namespace Rasterizer

#endif //_RENDERER_H_
