#include <Eigen/Core>
#include "Surface.hpp"

using namespace Eigen;

class Triangle : public Surface
{
public:
  typedef Matrix<unsigned int, 3, 1> Vector3u;

  Triangle();

  ~Triangle();

  bool Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const override;
  bool Intersect(const Ray& ray, float tMax, float& t) const override;
  bool Intersect(const Ray& ray, float tMax) const override;

  Vector3f GetNormal() const;
  Vector3f GetNormal(const Vector3f& p) const override;
private:
  Vector3u indices;
}
