#include "Testing.h"

using namespace std;

namespace triangleSpace
{
  void framebuffer_size_callback(GLFWwindow* window, int width, int height);
  void processInput(GLFWwindow *window);

  // settings
  const unsigned int SCR_WIDTH = 800;
  const unsigned int SCR_HEIGHT = 600;


  const char* vertexShaderSource =
    "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

  const char* fragmentShaderSourceOrange =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

  const char* fragmentShaderSourceGreen =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(0.26f, 0.957f, 0.28f, 1.0f);\n"
    "}\0";

  bool isFilling = true;
  bool keyPressed = false;

  // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
  // ---------------------------------------------------------------------------------------------------------
  void processInput(GLFWwindow *window)
  {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
    if (!keyPressed && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
      keyPressed = true;
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
      keyPressed = false;
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

int helloTriangle()
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
  GLFWwindow* window = glfwCreateWindow(triangleSpace::SCR_WIDTH, triangleSpace::SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, triangleSpace::framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }




#pragma region building and preparing shaders
  // ------------------------------
  // Compiling the vertex shader
  unsigned int vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &triangleSpace::vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Checking that the vertex shader compiled
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
  }

  // Compiling the fragment shader
  unsigned int fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &triangleSpace::fragmentShaderSourceOrange, NULL);
  glCompileShader(fragmentShader);

  // Checking that the fragment shader compiled
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
  }

  // Creating a shader program
  unsigned int shaderProgramOrange;
  shaderProgramOrange = glCreateProgram();

  // Attaching shaders to program
  glAttachShader(shaderProgramOrange, vertexShader);
  glAttachShader(shaderProgramOrange, fragmentShader);
  glLinkProgram(shaderProgramOrange);

  // Checking that the shader program linked
  glGetProgramiv(shaderProgramOrange, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgramOrange, 512, NULL, infoLog);
    cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << endl;
  }

  // Fragment shader no longer in use and we can now make new one
  glDeleteShader(fragmentShader);

  // Compiling the fragment shader
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &triangleSpace::fragmentShaderSourceGreen, NULL);
  glCompileShader(fragmentShader);

  // Checking that the fragment shader compiled
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
  }

  // Creating a shader program
  unsigned int shaderProgramGreen;
  shaderProgramGreen = glCreateProgram();

  // Attaching shaders to program
  glAttachShader(shaderProgramGreen, vertexShader);
  glAttachShader(shaderProgramGreen, fragmentShader);
  glLinkProgram(shaderProgramGreen);

  // Checking that the shader program linked
  glGetProgramiv(shaderProgramGreen, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgramGreen, 512, NULL, infoLog);
    cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << endl;
  }

  // Telling the process to use the shader program
  glUseProgram(shaderProgramOrange);

  // Now that the program is in use, we do not need the vertex and fragment shaders
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
#pragma endregion

#pragma region Everything in one
  //float vertices[] =
  //{
  //  0.5f,  0.5f, 0.0f,  // top right
  //   0.5f, -0.5f, 0.0f,  // bottom right
  //  -0.5f, -0.5f, 0.0f,  // bottom left
  //  -0.5f,  0.5f, 0.0f   // top left 
  //};
  ///* Drawing two triangles by using 6 vertices
  //float vertices[] =
  //{
  //   0.5f,  0.5f, 0.0f,
  //   0.5f, -0.5f, 0.0f,
  //  -0.5f,  0.5f, 0.0f,
  //   0.5f, -0.5f, 0.0f,
  //  -0.5f, -0.5f, 0.0f,
  //  -0.5f,  0.5f, 0.0f
  //};*/
  //unsigned int indices[] = {  // note that we start from 0!
  //  0, 1, 3,   // first triangle
  //  1, 2, 3    // second triangle
  //};
  //
  //// Preparing the vertex array object and giving it an id
  //unsigned int VAO;
  //glGenVertexArrays(1, &VAO);
  //glBindVertexArray(VAO);

  //// Assigning vertex buffer object an id
  //unsigned int VBO;
  //glGenBuffers(1, &VBO);
  //glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //// Assigning element buffer object an id
  //unsigned int EBO;
  //glGenBuffers(1, &EBO);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#pragma endregion

  float verticesLeft[] =
  {
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
  };
  float verticesRight[] =
  {
     0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
  };
  unsigned int VAOs[2], VBOs[2];
  glGenVertexArrays(2, VAOs); // we can also generate multiple VAOs or buffers at the same time
  glGenBuffers(2, VBOs);
  // first triangle setup
  // --------------------
  glBindVertexArray(VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesLeft), verticesLeft, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
  glEnableVertexAttribArray(0);
  // glBindVertexArray(0); // no need to unbind at all as we directly bind a different VAO the next few lines
  // second triangle setup
  // ---------------------
  glBindVertexArray(VAOs[1]);	// note that we bind to a different VAO now
  glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);	// and a different VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRight), verticesRight, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); // because the vertex data is tightly packed we can also specify 0 as the vertex attribute's stride to let OpenGL figure it out
  glEnableVertexAttribArray(0);

  // Translating Shader to OpenGL
  // ----------------------------
  // This will specify to OpenGL how it should be reading the vertices array that we 
  // created earlier. We bound the array to the GL_ARRAY_BUFFER and that is what
  // these attributes are set to.
  /*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
*/
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    triangleSpace::processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Drawing our triangle
    // --------------------
    glUseProgram(shaderProgramOrange);
    glBindVertexArray(VAOs[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glUseProgram(shaderProgramGreen);
    glBindVertexArray(VAOs[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    /*glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);*/

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }


  glDeleteVertexArrays(2, VAOs);
  glDeleteBuffers(2, VBOs);
  //glDeleteBuffers(1, &EBO);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}