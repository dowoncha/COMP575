#pragma once
#ifndef _RAST_BARY_
#define _RAST_BARY_

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Barycentric
{
public:
  float alpha, beta, gamma;
  int xMin, xMax;
  int yMin, yMax;
  glm::vec4 va, vb, vc;

  Barycentric(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c);

  // return a buffer containing x,y coordinates that should be drawn.
  // Should interpoation occur here?
  std::vector<glm::vec2> GetFragments();
private:
  // TODO: Use a GLM::vec3 here
  glm::vec3 betaCoef;   // Beta constants: betaX, betaY, beta0
  glm::vec3 gammaCoef;  // Gamma constants: gammaX, gammaY, gamma0

  // Calculate the bounding box for the 3 vertices
  void CalculateBounds();

  // Compute beta, gamma for abg
  void CalcBetaCoef();
  void CalcGammaCoef();

  void CalcAlpha();
};

#endif
