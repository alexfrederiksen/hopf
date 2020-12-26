#include <epoxy/gl.h>

#include <GLFW/glfw3.h>
#include <iostream>

#include "App.hpp"

/* main application */
App * app = nullptr;

void printError(int error, const char * desc)
{
  std::cout << "[Error]: " << desc << std::endl;
}

void handleResize(GLFWwindow * window, int width, int height)
{
  // std::cout << "Resizing to " << width << " by " << height << std::endl;
  if (app)
  {
    app->resize(width, height);
  } else
  {
    glViewport(0, 0, width, height);
  }

}

int main()
{
  GLFWwindow * window;

  glfwSetErrorCallback(printError);

  // initialize
  if (!glfwInit())
    return -1;

  glfwWindowHintString(GLFW_X11_CLASS_NAME, "floater");
  glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "floater");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // open window
  window = glfwCreateWindow(800, 600, "Manifold", nullptr, nullptr);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwSetFramebufferSizeCallback(window, handleResize);

  glfwMakeContextCurrent(window);

  {
    // construct app
    App app;
    ::app = &app;

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    app.resize(width, height);

    double lastTime = glfwGetTime();

    while(!glfwWindowShouldClose(window))
    {
      double currentTime = glfwGetTime();
      double deltaTime = currentTime - lastTime;
      lastTime = currentTime;

      //glClear(GL_COLOR_BUFFER_BIT);

      app.tick(deltaTime);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  glfwTerminate();
  return 0;
}
