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

#include <string>
#include <vector>
#include <array>
#include <cstdlib> 				// Needed for atoi
#include <cassert>
#include <cmath>
#include <algorithm>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp> // glm::cross, glm::normalize

#include "easylogging++.h"

#include "Scene.h"
#include "Utility.hpp"
#include "Barycentric.h"

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

static inline uint8_t Pixel_R(Pixel p) { return (p >> GPIXEL_SHIFT_R) & 0xFF; }
static inline uint8_t Pixel_G(Pixel p) { return (p >> GPIXEL_SHIFT_G) & 0xFF; }
static inline uint8_t Pixel_B(Pixel p) { return (p >> GPIXEL_SHIFT_B) & 0xFF; }
static inline uint8_t Pixel_A(Pixel p) { return (p >> GPIXEL_SHIFT_A) & 0xFF; }

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

class Renderer
{
private:
	class Vertex
	{
	public:
		glm::vec3 pos, color;

		Vertex(const glm::vec3& p, const glm::vec3& c) : pos(p), color(c) {}
	};
public:
	/**
		*  Provide the output buffer and the width and height of the screen
		*/
	Renderer(const Scene&);

	~Renderer();

	void Initialize(int argc, char* argv[]);

	void Render();

	void OutputToPPM(const std::string& filename) const;
private:
	void BufferInit();

	void DrawRow(int x1, int x2, int scanlineY);

	void DrawPixel(int x, int y, const glm::vec3& color);

	void linEval(int xl, int xr, int yl, int yr, const glm::vec3& color);

	glm::vec3 GammaEncode(const glm::vec3& color) const;

	Pixel ColorToPixel(const glm::vec3& color) const;

	glm::vec3 PinToUnit(const glm::vec3& color) const;

	/**
	 * Clear the entire color and depth buffers
	 */
	void ClearColorBuffer();
	void ClearDepthBuffer();
public:
	const Scene& scene;							// Should this be a reference?

	int ScreenWidth, ScreenHeight;

	// Buffers
	std::vector<glm::vec3> ColorBuffer;
	std::vector<float> DepthBuffer;

	size_t bufferSize;
};

} //end of namespace Rasterizer

#endif //_RENDERER_H_
