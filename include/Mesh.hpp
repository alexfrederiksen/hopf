#pragma once

#include <list>
#include <glm/mat4x4.hpp>

class Mesh
{
  unsigned int vao;
  unsigned int vbo;
  int vertexCount;
public:
  Mesh(float * vertices, int n);
  void draw();
  ~Mesh();
};

// used for instanced drawing (free performance!!!)
class MeshBatch
{

  std::list<glm::mat4> instances;
  bool dirty; // denotes when instances should be flushed to GPU

public:
  typedef std::list<glm::mat4>::iterator InstanceId;

  InstanceId addInstance(glm::mat4 && instance)
  {
    instances.push_front(std::move(instance));
    return instances.begin();
  }

  void removeInstance(InstanceId id)
  {
    instances.erase(id);
  }

  void draw();
};
