#include "Testing.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Particle.h"
#include "ParticleEmitter.h"
#include <vector>

using namespace std;

namespace mySpace
{
  void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  void processInput(GLFWwindow *window);
  void switchBetweenWireframe(GLFWwindow *window);

  // settings
  const unsigned int SCR_WIDTH = 800;
  const unsigned int SCR_HEIGHT = 600;

  const string vertexShaderPath = "Shaders/Particle.vert";
  const string fragmentShaderPath = "Shaders/Particle.frag";

  bool isFilling = true;
  bool keyPressed_c = false;

  // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
  // ---------------------------------------------------------------------------------------------------------
  void processInput(GLFWwindow *window)
  {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
    switchBetweenWireframe(window);
  }

  void switchBetweenWireframe(GLFWwindow *window)
  {
    if (!keyPressed_c && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
      keyPressed_c = true;
      if (isFilling)
      {
        isFilling = false;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      }
      else
      {
        isFilling = true;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      }
    }
    else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
    {
      keyPressed_c = false;
    }
  }

  // glfw: whenever the window size changed (by OS or user resize) this callback function executes
  // ---------------------------------------------------------------------------------------------
  void framebuffer_size_callback(GLFWwindow* window, int width, int height)
  {
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
  }
}

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
  GLFWwindow* window = glfwCreateWindow(mySpace::SCR_WIDTH, mySpace::SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, mySpace::framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  //Shader particleShader(mySpace::vertexShaderPath, mySpace::fragmentShaderPath);



  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);


  ParticleEmitter pm(500, mySpace::vertexShaderPath, mySpace::fragmentShaderPath);
  pm.ConfigureShader(mySpace::SCR_WIDTH, mySpace::SCR_HEIGHT);

  GLfloat deltaTime;
  GLfloat lastFrame = 0;
  GLfloat currentFrame = 0;

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // Updating time
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;


    // input
    // -----
    mySpace::processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // Updating our particles
    pm.Update(deltaTime, 2);
    // Drawing our particles
    pm.Draw();


    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}