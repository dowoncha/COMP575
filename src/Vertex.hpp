#pragma once

#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>

class Vertex
{
public:
	glm::vec4 pos, color, normal;

	Vertex() : pos(0.0f), color(0.0f), normal(0.0f) {}

	Vertex(const glm::vec4& p, const glm::vec4& c) : pos(p), color(c), normal(0.0f) { }

	Vertex(const glm::vec4& p, const glm::vec4& c, const glm::vec4& n) : pos(p), color(c), normal(n) { }

	Vertex(const Vertex& a) : pos(a.pos), color(a.color), normal(a.normal) {}

	void Transform(const glm::mat4x4& transform)
	{
		pos = transform * pos;
		normal = transform * normal; //glm::transpose(glm::inverse(transform)) * normal;
	}
};
