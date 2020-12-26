#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class Fiber
{
  unsigned int vao;
  unsigned int vbo;

  int vertexCount;

public:
  Fiber();
  void compute(double phi, double psi, int res);
  void computeSmooth(double phi, double psi, float maxEdge);
  void draw();
  ~Fiber();
};
