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

glm::vec3 currentExplosionColor(1, 0, 0);

Firework * CreateFirework();
void RespawnFirework(Firework *firework, const bool stopped = false);


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
      
      for (Firework * firework : f)
        RespawnFirework(firework);
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

void RespawnFirework(Firework *firework, const bool stopped)
{
  glm::vec3 origin = randomVec3(glm::vec3(-100.0f, -80.0f, -180.0f), glm::vec3(100.0f, -70.0f, -170.0f));
  glm::vec3 initialVelocity = randomVec3(glm::vec3(-10.0f, 45.0f, -10.0f), glm::vec3(10.0f, 75.0f, 10.0f));

  if (!stopped)
    firework->Stop();
  firework->Start(origin, initialVelocity, currentExplosionColor);
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
  
  
  
  
  GLuint numFireworks = 10;
  vector<Firework *> fireworks;
  for (size_t i = 0; i < numFireworks; i++)
    fireworks.push_back(CreateFirework());
  

  GLfloat deltaTime;
  GLfloat lastFrame = 0;
  GLfloat currentFrame = 0;

  size_t curFireworkInd = 0;
  GLfloat timeToNextFirework = 0;
  GLfloat minTimeToNextFirework = 0.5f;
  GLfloat maxTimeToNextFirework = 1.5f;

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // Spawning a firework
    if (timeToNextFirework <= 0)
    {
      RespawnFirework(fireworks[curFireworkInd++]);
      if (curFireworkInd == numFireworks)
        curFireworkInd = 0;
      timeToNextFirework = randomFloat(minTimeToNextFirework, maxTimeToNextFirework);
    }
    else
      timeToNextFirework -= deltaTime;


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