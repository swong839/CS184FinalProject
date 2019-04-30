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
  void processInput(GLFWwindow *window, vector<ParticleEmitter *> &PEs);
  void switchBetweenWireframe(GLFWwindow *window);
  void restartParticleSystem(GLFWwindow *window, vector<ParticleEmitter *> &PEs);

  // settings
  const unsigned int SCR_WIDTH = 800;
  const unsigned int SCR_HEIGHT = 600;

  const string vertexShaderPath = "Shaders/Particle.vert";
  const string fragmentShaderPath = "Shaders/Particle.frag";

  bool isFilling = true;
  bool keyPressed_c = false;
  bool keyPressed_r = false;

  // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
  // ---------------------------------------------------------------------------------------------------------
  void processInput(GLFWwindow *window, vector<ParticleEmitter *> &PEs)
  {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
    switchBetweenWireframe(window);
    restartParticleSystem(window, PEs);
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

  void restartParticleSystem(GLFWwindow *window, vector<ParticleEmitter *> &PEs)
  {
    if (!keyPressed_r && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
      keyPressed_r = true;
      
      for (ParticleEmitter *PE : PEs)
      {
        PE->Stop();
        PE->Start();
      }
    }
    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
    {
      keyPressed_r = false;
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

  
  vector<ParticleEmitter *> PEs;

  ParticleEmitter pm(500, mySpace::vertexShaderPath, mySpace::fragmentShaderPath);
  pm.ConfigureShader(mySpace::SCR_WIDTH, mySpace::SCR_HEIGHT);
  glm::vec3 pmOrigin(0.0f, 0.0f, -20.0f);  
  GLfloat pmSphereRadius = 2.0f;
  GLfloat pmParticleSpawnRate = 50;
  GLboolean pmEmitOverTime = false;
  GLboolean pmIsSpinningTrail = false;

  glm::vec3 pmStartSize(0.5f);
  glm::vec3 pmStartColor(1.0f, 0.0f, 0.0f);
  GLfloat pmStartLifetime = 5.0f;
  GLfloat pmStartSpeed = 20.0f;

  glm::vec3 pmGravity(0.0f, -9.8f, 0.0f);
  
  pm.SetEmitterVariables(
    pmOrigin, pmSphereRadius, pmParticleSpawnRate, pmEmitOverTime, pmIsSpinningTrail,
    pmStartSize, pmStartColor, pmStartLifetime, pmStartSpeed,
    pmGravity);
  PEs.push_back(&pm);
  

  ParticleEmitter pm2(1000, mySpace::vertexShaderPath, mySpace::fragmentShaderPath);
  pm2.ConfigureShader(mySpace::SCR_WIDTH, mySpace::SCR_HEIGHT);
  pmOrigin = glm::vec3(0.0f, -20.0f, -5.0f);
  pmSphereRadius = 0.5f;
  pmParticleSpawnRate = 100;
  pmEmitOverTime = true;
  pmIsSpinningTrail = true;

  pmStartSize = glm::vec3(0.1f);
  pmStartColor = glm::vec3(0.529f, 0.808f, 0.922f);
  pmStartLifetime = 5.0f;
  pmStartSpeed = 10.0f;

  pmGravity = glm::vec3(0.0f, -98.0f, 0.0f);

  pm2.SetEmitterVariables(
    pmOrigin, pmSphereRadius, pmParticleSpawnRate, pmEmitOverTime, pmIsSpinningTrail,
    pmStartSize, pmStartColor, pmStartLifetime, pmStartSpeed,
    pmGravity);
  PEs.push_back(&pm2);

  
  for (ParticleEmitter *PE : PEs)
    PE->Start();


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
    mySpace::processInput(window, PEs);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    for (ParticleEmitter *PE : PEs)
    {
      // Updating our particles
      PE->Update(deltaTime);
      // Drawing our particles
      PE->Draw();
    }


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