#pragma once

#include "Mesh.hpp"
#include "Hopf.hpp"

#include <vector>
#include <memory>

#include <glm/mat4x4.hpp>

class App
{
  int width;
  int height;

  unsigned int shaderProgram;
  int shaderProjUniform;
  int shaderColorUniform;

  std::vector<std::unique_ptr<Fiber>> fibers;
  Mesh triangeMesh;

  glm::mat4 proj;
  glm::mat4 view;
  glm::mat4 combined;


public:
  App();
  void lineOfFibers(float phi1, float psi1,
                    float phi2, float psi2,
                    int n);
  void randomizeFibers();
  void resize(int width, int height);
  void tick(double deltaTime);
  ~App();
};
