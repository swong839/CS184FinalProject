#include "Testing.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Particle.h"
#include "ParticleEmitter.h"
#include "Firework.h"
#include <vector>

using namespace std;

namespace mySpace
{
  void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  void processInput(GLFWwindow *window, vector<Firework*> &f);
  void switchBetweenWireframe(GLFWwindow *window);
  void restartParticleSystem(GLFWwindow *window, vector<Firework*> &f);

  // settings
  const unsigned int SCR_WIDTH = 1920;
  const unsigned int SCR_HEIGHT = 1080;

  const string vertexShaderPath = "Shaders/Particle.vert";
  const string fragmentShaderPath = "Shaders/Particle.frag";

  bool isFilling = true;
  bool keyPressed_c = false;
  bool keyPressed_r = false;

  // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
  // ---------------------------------------------------------------------------------------------------------
  void processInput(GLFWwindow *window, vector<Firework*> &f)
  {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
    switchBetweenWireframe(window);
    restartParticleSystem(window, f);
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

  void restartParticleSystem(GLFWwindow *window, vector<Firework*> &f)
  {
    if (!keyPressed_r && glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
      keyPressed_r = true;
      
      glm::vec3 origin(0.0f);
      glm::vec3 initialVelocity(0.0f);
      for (Firework * firework : f)
      {
        origin = randomVec3(glm::vec3(-25.0f, -50.0f, -180.0f), glm::vec3(25.0f, -45.0f, -170.0f));
        initialVelocity = randomVec3(glm::vec3(-10.0f, 45.0f, -10.0f), glm::vec3(10.0f, 55.0f, 10.0f));

        firework->Stop();
        firework->Start(origin, initialVelocity);
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

Firework * CreateFirework()
{
  std::string vsp = mySpace::vertexShaderPath;
  std::string fsp = mySpace::fragmentShaderPath;
  GLfloat gravity = -9.8f;
  glm::vec3 windForces(-10, 0, 0);
  ExplosionType et = DEFAULT_ET;
  TrailType tt = DEFAULT_TT;
  SmokeType st = DEFAULT_ST;
  Firework *f = new Firework(gravity, windForces, et, vsp, fsp, tt, vsp, fsp, st, vsp, fsp);
  f->ConfigureShaders(mySpace::SCR_WIDTH, mySpace::SCR_HEIGHT);
  return f;
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
  

  //glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  
  
  
  
  GLuint numFireworks = 1;
  vector<Firework *> fireworks;
  for (size_t i = 0; i < numFireworks; i++)
    fireworks.push_back(CreateFirework());
  glm::vec3 origin(0.0f);
  glm::vec3 initialVelocity(0.0f);
  for (Firework *f : fireworks)
  {
    origin = randomVec3(glm::vec3(-25.0f, -50.0f, -180.0f), glm::vec3(25.0f, -45.0f, -170.0f));
    initialVelocity = randomVec3(glm::vec3(-10.0f, 45.0f, -10.0f), glm::vec3(10.0f, 55.0f, 10.0f));
    f->Start(origin, initialVelocity);
  }


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
    mySpace::processInput(window, fireworks);

    // render
    // ------
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (Firework *f : fireworks)
    {
      f->Update(deltaTime);
      f->Draw();
    }


    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  for (Firework *f : fireworks)
    delete f;
  fireworks.clear();


  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}