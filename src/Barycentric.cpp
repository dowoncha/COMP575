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
  std::vector<glm::vec2> pixels;

  beta = betaCoef.x * xMin + betaCoef.y * yMin + betaCoef.z;
  gamma = gammaCoef.x * xMin + gammaCoef.y * yMin + gammaCoef.z;
  CalcAlpha();

  float betaRow = beta;
  float gammaRow = gamma;

  float alphaT;
  float betaT;
  float gammaT;

  for (int y = yMin; y <= yMax; ++y)
  {
    betaT = betaRow;
    gammaT = gammaRow;
    alphaT = 1.0f - betaRow - gammaRow;

    for (int x = xMin; x <= xMax; ++x)
    {
      if (alphaT >= 0.0f && betaT >= 0.0f && gammaT >= 0.0f)
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

  return pixels;
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
