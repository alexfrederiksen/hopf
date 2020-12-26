#include "Mesh.hpp"

#include <epoxy/gl.h>
#include <epoxy/gl_generated.h>

Mesh::Mesh(float * vertices, int n) : vertexCount(n)
{
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 3 * n * sizeof(float), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Mesh::draw()
{
  glBindVertexArray(vao);
  glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

Mesh::~Mesh()
{
  glDeleteBuffers(1, &vbo);
  glDeleteVertexArrays(1, &vao);
}
