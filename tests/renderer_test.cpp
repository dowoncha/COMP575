#include "Renderer.h"
#include "gtest/gtest.h"

namespace
{
class RendererTest : public testing::Test
{
protected:
  Renderer(const Scene&)
  {

  }

  virtual ~Renderer()
  {}

  virtual void SetUp()
  {
  }

  virtual void TearDown()
  {}

  Renderer rend;
  Scene scene;
};

TEST_F(RendererTest, DefaultConstructor)
{

}

TEST_F(RendererTest, SetupModelTransform)
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
