#pragma once
#ifndef _RAST_BARY_
#define _RAST_BARY_

#include <vector>
#include <cmath>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Barycentric
{
public:
  float alpha, beta, gamma;
  int xMin, xMax;
  int yMin, yMax;
  glm::vec4 va, vb, vc;

  Barycentric(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c);

  /**
   *  return a buffer containing x,y coordinates that should be drawn.
   */
  std::vector<glm::vec2> GetInterior();

private:
  // TODO: Use a GLM::vec3 here
  float beta0, betaX, betaY;
  float gamma0, gammaX, gammaY;

  /**
   *  Compute beta, gamma
   */
  void ComputeConstants();
};

#endif
