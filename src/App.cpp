#include "App.hpp"

#include "Colors.hpp"

#include <epoxy/gl.h>
#include <epoxy/gl_generated.h>

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
#include <functional>
#include <random>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>


float vertices[] = {
  -0.5f, -0.5f, 0.0f,
   0.5f, -0.5f, 0.0f,
   0.0f,  0.5f, 0.0f
};

void loadShader(std::string filename, unsigned int shader)
{
  // load file into string
  std::ifstream file(filename);
  std::string source((std::istreambuf_iterator<char>(file)),
                     (std::istreambuf_iterator<char>()));

  // load string into shader and compile
  const char * cSource = source.c_str();
  glShaderSource(shader, 1, &cSource, nullptr);
  glCompileShader(shader);

  // check if the shader compiled
  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cout << "Shader failed to compile: " << infoLog << std::endl;
  }
}

void buildShaderProgram(unsigned int program, unsigned int vertex,
                        unsigned int fragment)
{
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);

  // check if the shader compiled
  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program, 512, nullptr, infoLog);
    std::cout << "Shader program failed to link: " << infoLog << std::endl;
  }
}


App::App() : triangeMesh(vertices, 3)
{
  view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

//  lineOfFibers(2, 2, 2, 6, 20);
//  lineOfFibers(3, 2, 3, 6, 20);
//  lineOfFibers(-1, 2, -1, 6, 20);
//  lineOfFibers(-5, 2, -5, 6, 20);

  lineOfFibers(-2, -2, -2, 2, 40);
  lineOfFibers(-1, -2, -1, 2, 40);
  lineOfFibers(0, -2, 0, 2, 40);
  lineOfFibers(1, -2, 1, 2, 40);
  lineOfFibers(2, -2, 2, 2, 40);

  //lineOfFibers(3, -2, 3, 2, 40);

  //randomizeFibers();

  // initialize shaders
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  loadShader("shaders/basic.vert", vertexShader);
  loadShader("shaders/basic.frag", fragmentShader);

  shaderProgram = glCreateProgram();
  buildShaderProgram(shaderProgram, vertexShader, fragmentShader);

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  shaderProjUniform = glGetUniformLocation(shaderProgram, "proj");
  shaderColorUniform = glGetUniformLocation(shaderProgram, "fiberColor");

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(5.0f);
  glEnable(GL_DEPTH_TEST);
}


const bool useSmoothing = false;

void App::randomizeFibers()
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_real_distribution<float> distribution;
  auto randfloat = std::bind(distribution, generator);

  for (auto & fiber : fibers)
  {
    if (useSmoothing)
    {
      fiber->computeSmooth(randfloat() * 2 * M_PI,
                           randfloat() * M_PI, 0.1f);
    } else
    {
      fiber->compute(randfloat() * 2 * M_PI,
                     randfloat() * M_PI, 100);
    }
  }
}

void App::lineOfFibers(float phi1, float psi1, float phi2, float psi2, int n)
{
  for (int i = 0; i < n; i++)
  {
    auto fiber = std::make_unique<Fiber>();

    float t = ((float) i) / n;

    if (useSmoothing)
    {
      fiber->computeSmooth(phi1 * (1 - t) + phi2 * t,
                           psi1 * (1 - t) + psi2 * t, 0.01f);
    } else
    {
      fiber->compute(phi1 * (1 - t) + phi2 * t,
                     psi1 * (1 - t) + psi2 * t, 40);
    }

    fibers.push_back(std::move(fiber));
  }
}

void App::resize(int width, int height)
{
  this->width = width;
  this->height = height;

  glViewport(0, 0, width, height);

  proj = glm::perspective(glm::radians(45.0f),
                          ((float) width) / ((float) height),
                          0.1f, 100.0f);
  combined = proj * view;
}

void App::tick(double deltatime)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  view = glm::rotate(view, 0.01f, glm::vec3(1.0f, 1.0f, 1.0f));
  combined = proj * view;

  glUseProgram(shaderProgram);

  glUniformMatrix4fv(shaderProjUniform, 1, GL_FALSE,
                     glm::value_ptr(combined));

  //triangeMesh.draw();

  // draw the fibers
  int i = 0;
  for (auto & fiber : fibers)
  {
    rgb color = hsv2rgb({ ((float) i / fibers.size()) * 360.0,
                                 1.0, 1.0});

    glUniform3f(shaderColorUniform, color.r, color.g, color.b);

    fiber->draw();
    i++;
  }
}

App::~App()
{
  glDeleteProgram(shaderProgram);
}
