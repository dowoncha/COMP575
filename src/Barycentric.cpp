#include "Barycentric.h"

Barycentric::Barycentric(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c) :
  va(a),
  vb(b),
  vc(c)
{
  // Calculate the bounding box
  xMax = (int) std::ceil((std::max)(a.x, (std::max)(b.x, c.x)));
  xMin = (int) std::floor((std::min)(a.x, (std::min)(b.x, c.x)));
  yMax = (int) std::ceil((std::max)(a.y, (std::max)(b.y, c.y)));
  yMin = (int) std::floor((std::min)(a.y, (std::min)(b.y, c.y)));

  ComputeConstants();
}

std::vector<glm::vec2> Barycentric::GetInterior()
{
  std::vector<glm::vec2> pixels;

  glm::vec2 target(xMin, yMin);

  beta = beta0 + betaX * target.x + betaY * target.y;
  gamma = gamma0 + gammaX * target.x + gammaY * target.y;
  alpha = 1 - beta - gamma;

  float betaRow = beta;
  float gammaRow = gamma;

  for (int y = yMin; y <= yMax; ++y)
  {
    beta = betaRow;
    gamma = gammaRow;
    alpha = 1.0f - beta - gamma;

    for (int x = xMin; x <= xMax; ++x)
    {
      if (alpha >= 0 && beta >= 0 && gamma >= 0)
      {
        pixels.push_back(glm::vec2(x, y));
      }
    }
  }

  return pixels;
}

void Barycentric::ComputeConstants()
{
  float beta_denominator =
    (va.y - vc.y) * vb.x +
    (vc.x - va.x) * vb.y +
    va.x * vc.y - vc.x * va.y;

  beta0 = (va.x * vc.y - vc.x * va.y) / beta_denominator;
  betaX = (va.y - vc.y) / beta_denominator;
  betaY = (vc.x - va.x) / beta_denominator;


  float gamma_denominator =
    (va.y - vb.y) * vc.x +
    (vb.x - va.x) * vc.y +
    va.x * vb.y - vb.x * va.y;

  gamma0 = (va.x * vb.y - vb.x * va.y) / gamma_denominator;
  gammaX = (va.y - vb.y) / gamma_denominator;
  gammaY = (vb.x - va.x) / gamma_denominator;

}
