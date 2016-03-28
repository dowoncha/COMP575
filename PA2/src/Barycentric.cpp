#include "Barycentric.h"

Barycentric::Barycentric(const glm::vec4& a, const glm::vec4& b, const glm::vec4& c) :
  va(a),
  vb(b),
  vc(c)
{
    CalculateBounds();
    CalcBetaCoef();
    CalcGammaCoef();
}

std::vector<glm::vec2> Barycentric::GetFragments()
{
  /*
  std::vector<glm::vec2> pixels;

  SetCurrent(xMin, yMin);

  float betaRow = beta;
  float gammaRow = gamma;

  float alphaX;
  float betaX;
  float gammaX;

  for (int y = yMin; y <= yMax; ++y)
  {
    betaX = betaRow;
    gammaX = gammaRow;
    alphaX = 1.0f - betaRow - gammaRow;

    for (int x = xMin; x <= xMax; ++x)
    {
      if (alphaX >= 0.0f && betaX >= 0.0f && gammaT >= 0.0f)
      {
        pixels.push_back(glm::vec2(x, y));
      }
      betaT += betaCoef.x;
      gammaT += gammaCoef.x;
      alphaT = 1.0f - betaT - gammaT;
    }

    betaRow += betaCoef.y;
    gammaRow += gammaCoef.y;
  }
  */
  return std::vector<glm::vec2>();
}

void Barycentric::CalculateBounds()
{
  // Calculate the bounding box
  xMax = (int) std::ceil ((std::max)(va.x, (std::max)(vb.x, vc.x)));
  xMin = (int) std::floor((std::min)(va.x, (std::min)(vb.x, vc.x)));
  yMax = (int) std::ceil ((std::max)(va.y, (std::max)(vb.y, vc.y)));
  yMin = (int) std::floor((std::min)(va.y, (std::min)(vb.y, vc.y)));

  xMin = (std::max)(0, xMin);
  xMax = (std::min)(512, xMax);
  yMin = (std::max)(0, yMin);
  yMax = (std::min)(512, yMax);
}

void Barycentric::CalculateBaryCoordinates()
{
  beta = betaCoef.x * current.x + betaCoef.y * current.y + betaCoef.z;
  gamma = gammaCoef.x * current.x + gammaCoef.y * current.y + gammaCoef.z;
  alpha = 1.0f - beta - gamma;
}

void Barycentric::SetCurrent(float x, float y)
{
  current.x = x;
  current.y = y;

  CalculateBaryCoordinates();
}

void Barycentric::SetCurrent(int x, int y)
{
  current.x = x;
  current.y = y;

  CalculateBaryCoordinates();
}

void Barycentric::SetCurrent(const glm::vec2& c)
{
  current.x = c.x;
  current.y = c.y;

  CalculateBaryCoordinates();
}

void Barycentric::CalcAlpha()
{
    alpha = 1.0f - beta - gamma;
}

void Barycentric::CalcBetaCoef()
{
  float denom =
    (va.y - vc.y) * vb.x +
    (vc.x - va.x) * vb.y +
    va.x * vc.y - vc.x * va.y;

  betaCoef.x = (va.y - vc.y) / denom;
  betaCoef.y = (vc.x - va.x) / denom;
  betaCoef.z = (va.x * vc.y - vc.x * va.y) / denom;
}

void Barycentric::CalcGammaCoef()
{
  float denom =
    (va.y - vb.y) * vc.x +
    (vb.x - va.x) * vc.y +
    va.x * vb.y - vb.x * va.y;

  gammaCoef.x = (va.y - vb.y) / denom;
  gammaCoef.y = (vb.x - va.x) / denom;
  gammaCoef.z = (va.x * vb.y - vb.x * va.y) / denom;
}
