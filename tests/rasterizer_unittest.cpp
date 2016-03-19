#include "Rasterizer.h"
#include "gtest/gtest.h"

namespace
{

class RasterizerTest : public testing::Test
{
protected:
  Rasterizer(const Scene)
  {

  }

  virtual ~Rasterizer()
  {}

  virtual void SetUp()
  {

  }

  virtual void TearDown()
  {}

  Rasterizer rast;
  Scene scene;
};

TEST_F(RasterizerTest, DefaultConstructor)
{
  EXPECT_EQ(glm::mat4x4(0.0f), scene.ModelTransform);
}

TEST_F(QueueTest, SetupModelTransform)
{
  scene.SetupModelTransform(2, glm::vec3(0.0f, 0.0f, -7.0f));

  // I should do this by hand...
  glm::mat4x4 scale = glm::scale(glm::mat4x4(1.0f), glm::vec3(2.0f));
  glm::mat4x4 trans = glm::translate(glm::mat4x4(1.0f), glm::vec3(0.0f, 0.0f, -7.0f));

  glm::mat4x4 res = trans * scale;

  EXPECT_EQ(res, scene.ModelTransform));
}

} //namespace

int main(int argc, char** arv)
{
    testing::InitGoogleTest(&argc, arv);
    return RUN_ALL_TESTS
}
