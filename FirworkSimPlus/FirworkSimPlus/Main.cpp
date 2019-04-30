#include "Testing.h"
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Particle.h"
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

GLuint nr_particles = 500;
std::vector<Particle> particles;
GLuint FirstUnusedParticle();
void RespawnParticle(Particle &particle, glm::vec2 initialPos, glm::vec2 offset, glm::vec2 initialVelocity);




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

  Shader particleShader(mySpace::vertexShaderPath, mySpace::fragmentShaderPath);


  float vertices[] =
  {
    // positions    // texture coords
    0.5f,  0.5f,    1.0f, 1.0f,   // top right
    0.5f, -0.5f,    1.0f, 0.0f,   // bottom right
   -0.5f, -0.5f,    0.0f, 0.0f,   // bottom left
   -0.5f,  0.5f,    0.0f, 1.0f    // top left 
  };
  unsigned int indices[] =
  {
    0, 1, 3,
    1, 2, 3
  };

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // triangle setup
  // --------------------
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // position and texture attributes
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);


  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


  unsigned int texture1;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping/filtering options (on the currently bound texture object)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load and generate texture
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data = stbi_load("Textures/MyParticle.png", &width, &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    cout << "Failed to load image." << endl;
    return -1;
  }
  stbi_image_free(data);

  particleShader.use();
  glUniform1i(glGetUniformLocation(particleShader.ID, "sprite"), 0);

  // Transform from local to world space
  glm::mat4 model = glm::mat4(1.0);

  // Transform from world space to camera space
  glm::mat4 view = glm::mat4(1.0f);
  // note that we're translating the scene in the reverse direction of where we want to move
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  int viewLoc = glGetUniformLocation(particleShader.ID, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  // Tranform from camera space to project space - essentially make sure the 3D scene LOOKS 3D
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), (float)mySpace::SCR_WIDTH / (float)mySpace::SCR_HEIGHT, 0.1f, 100.0f);
  int projectionLoc = glGetUniformLocation(particleShader.ID, "projection");
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);



  for (GLuint i = 0; i < nr_particles; i++)
    particles.push_back(Particle());



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


    // Spawn new particles
    GLuint nr_new_particles = 2;
    // Add new particles
    for (GLuint i = 0; i < nr_new_particles; i++)
    {
      GLuint unusedParticle = FirstUnusedParticle();
      RespawnParticle(particles[unusedParticle], glm::vec2(0.0f), glm::vec2(0.0f), glm::vec2(0.0f));
    }
    // Update all particles
    for (GLuint i = 0; i < nr_particles; i++)
    {
      Particle &p = particles[i];
      p.Life -= deltaTime;
      if (p.Life > 0.0f)
      {
        p.Position -= p.Velocity * deltaTime;
        p.Color -= deltaTime * 2.5f;
      }
    }


    // Drawing our triangle
    // --------------------
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    particleShader.use();
    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.1f));
    particleShader.setMat4("model", model);

    for (Particle particle : particles)
    {
      if (particle.Life > 0.0f)
      {
        particleShader.setVec2("offset", particle.Position);
        particleShader.setVec4("color", particle.Color);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
      }
    }
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);


  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}



GLuint lastUsedParticle = 0;
GLuint FirstUnusedParticle()
{
  // Search from last used particle, this will usually return almost instantly
  for (GLuint i = lastUsedParticle; i < nr_particles; ++i)
  {
    if (particles[i].Life <= 0.0f)
    {
      lastUsedParticle = i;
      return i;
    }
  }
  // Otherwise, do a linear search
  for (GLuint i = 0; i < lastUsedParticle; ++i) 
  {
    if (particles[i].Life <= 0.0f)
    {
      lastUsedParticle = i;
      return i;
    }
  }
  // Override first particle if all others are alive
  lastUsedParticle = 0;
  return 0;
}


void RespawnParticle(Particle &particle, glm::vec2 initialPos, glm::vec2 offset, glm::vec2 initialVelocity)
{
  GLfloat random = ((rand() % 100) - 50) / 10.0f;
  GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
  particle.Position = initialPos + random + offset;
  particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.Life = 1.0f;
  particle.Velocity = initialVelocity * 0.1f;
}
