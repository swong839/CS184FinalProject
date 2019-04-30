#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(const GLuint nr_particles, const std::string vertexPath, const std::string fragmentPath)
{
  SetupQuad();  

  shader = new Shader(vertexPath, fragmentPath);
  SetupTexture();

  this->nr_particles = nr_particles;
  for (GLuint i = 0; i < nr_particles; i++)
    particles.push_back(Particle());

  lastUsedParticle = 0;
}

ParticleEmitter::~ParticleEmitter()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  delete shader;
}

void ParticleEmitter::SetupQuad()
{
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
}

void ParticleEmitter::SetupTexture()
{
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
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
    std::cout << "Failed to load image." << std::endl;
    return;
  }
  stbi_image_free(data);

  shader->use();
  shader->setTexture("sprite", 0);
}

void ParticleEmitter::ConfigureShader(float width, float height)
{
  shader->use();

  // Transform from world space to camera space
  glm::mat4 view = glm::mat4(1.0f);
  // note that we're translating the scene in the reverse direction of where we want to move
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
  int viewLoc = glGetUniformLocation(shader->ID, "view");
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  // Tranform from camera space to project space - essentially make sure the 3D scene LOOKS 3D
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 100.0f);
  int projectionLoc = glGetUniformLocation(shader->ID, "projection");
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void ParticleEmitter::Update(const GLfloat deltaTime, const GLuint nr_new_particles)
{
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
}

void ParticleEmitter::Draw() const
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  shader->use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::scale(model, glm::vec3(0.1f));
  shader->setMat4("model", model);

  for (Particle particle : particles)
  {
    if (particle.Life > 0.0f)
    {
      shader->setVec2("offset", particle.Position);
      shader->setVec4("color", particle.Color);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    }
  }
  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GLuint ParticleEmitter::FirstUnusedParticle()
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

void ParticleEmitter::RespawnParticle(Particle &particle, glm::vec2 initialPos, glm::vec2 offset, glm::vec2 initialVelocity)
{
  GLfloat random = ((rand() % 100) - 50) / 10.0f;
  GLfloat rColor = 0.5 + ((rand() % 100) / 100.0f);
  particle.Position = initialPos + random + offset;
  particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
  particle.Life = 1.0f;
  particle.Velocity = initialVelocity * 0.1f;
}