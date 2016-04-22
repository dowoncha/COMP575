class Plane : public Surface
{
private:
  Vector3f Normal;
public:
  Plane::Plane(const Vector3f& pos, const Vector3f& normal, const Material& mat) :
    Surface(mat),
    Normal(normal.normalized())
  {
    Position = pos;
  }

  Plane::~Plane()
  { }

  bool Plane::Intersect(const Ray& ray, float tMax, float& t, Vector3f& Point) const override
  {
    float denom = glm::dot(Normal, ray.Direction);

    if (std::fabs(denom) > 1e-6)
    {
      t = glm::dot(Normal, Position - ray.Position) / denom;
      if (t >= 0.0f)
      {
        Point = ray.Evaluate(t);
        return true;
      }
    }

    return false;
  }

  bool Plane::Intersect(const Ray& ray, float tMax, float& t) const override
  {
    float denom = glm::dot(Normal, ray.Direction);

    if (std::fabs(denom) > 1e-6)
    {
      t = glm::dot(Normal, (Position - ray.Position)) / denom;
      if (t > 0.0f)
      {
        return true;
      }
    }

    return false;
  }

  bool Plane::Intersect(const Ray& ray, float tMax) const override
  {
    float denom = glm::dot(Normal, ray.Direction);

    if (std::fabs(denom) > 1e-6)
    {
      float t = glm::dot(Normal, (Position - ray.Position)) / denom;
      if (t >= 0.0f)
      {
        return true;
      }
    }

    return false;
  }

  Vector3f GetNormal() const override
  {
    return  Normal;
  }

  Vector3f GetNormal(const Vector3f& p) const override
  {
    return Normal;
  }
};
