#include "Hopf.hpp"

#include <vector>
#include <cmath>
#include <iostream>

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <epoxy/gl.h>

glm::vec4 fiberate(double phi, double psi, double t)
{
  return glm::vec4(std::cos((phi + t) / 2) * std::sin(psi / 2),
                   std::sin((phi + t) / 2) * std::sin(psi / 2),
                   std::cos((t - phi) / 2) * std::cos(psi / 2),
                   std::sin((t - phi) / 2) * std::cos(psi / 2));
}

glm::vec3 stereoProject(glm::vec4 v)
{
  return glm::vec3(v.x / (1 - v.w),
                   v.y / (1 - v.w),
                   v.z / (1 - v.w));
}

glm::vec3 ballProject(glm::vec3 v)
{
  float c = 1 / (1 + glm::length(v));
  return glm::vec3(v.x * c, v.y * c, v.z * c);
}


Fiber::Fiber()
{
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

glm::vec3 computeFiber(double phi, double psi, float t)
{
  // compute 4d point on great circle
  glm::vec4 f = fiberate(phi, psi, t * M_PI * 4);

  glm::vec3 s = stereoProject(f);

  glm::vec3 p = ballProject(s);
  return p;
}

void Fiber::computeSmooth(double phi, double psi, float maxEdge)
{
  float t = 0;

  std::vector<float> vertices;
  while(t < 1.0f)
  {
    glm::vec3 p = computeFiber(phi, psi, t);
    vertices.push_back(p.x);
    vertices.push_back(p.y);
    vertices.push_back(p.z);

    // perform backtracking until we get a vertex within desired distance
    float dt = 0.5f;
    while (glm::distance(p, computeFiber(phi, psi, t + dt)) > maxEdge)
    {
      dt /= 2.0f;
    }

    t += dt;
  }

  vertexCount = vertices.size() / 3;

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(),
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Fiber::compute(double phi, double psi, int res)
{
  vertexCount = res;

  std::vector<float> vertices;
  vertices.reserve(res);
  for (int i = 0; i < res; i++)
  {
    float t = ((float) i / res);

    glm::vec3 p = computeFiber(phi, psi, t);

    // and push coords to our vertex list
    vertices.push_back(p.x);
    vertices.push_back(p.y);
    vertices.push_back(p.z);
  }

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 3 * res * sizeof(float), vertices.data(),
               GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Fiber::draw()
{
  if (vertexCount <= 0) throw "Uncomputed fiber draw";

  glBindVertexArray(vao);
  glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
}

Fiber::~Fiber()
{
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}
