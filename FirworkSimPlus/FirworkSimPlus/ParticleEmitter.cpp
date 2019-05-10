#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(const GLuint nr_particles, const std::string vertexPath, const std::string fragmentPath)
{
  isPlaying = false;


  // Emitter object variables
  timeToNextParticle = 0;


  // Particle variables
  this->nr_particles = nr_particles;
  for (GLuint i = 0; i < nr_particles; i++)
    particles.push_back(Particle());

  // Shader variables
  SetupQuad();

  shader = new Shader(vertexPath, fragmentPath);
  SetupTexture();
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
     0.0f,   0.0f,     0.5f,  0.5f,   // center
     0.5f,   0.5f,     1.0f,  1.0f,   // top right
     0.5f,  -0.5f,     1.0f,  0.0f,   // bottom right
    -0.5f,  -0.5f,     0.0f,  0.0f,   // bottom left
    -0.5f,   0.5f,     0.0f,  1.0f    // top left 
  };
  unsigned int indices[] =
  {
    0, 1, 2,
    0, 2, 3,
    0, 3, 4,
    0, 4, 1
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // triangle setup
  // --------------------
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);
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
  shader->setMat4("view", view);
  // Tranform from camera space to project space - essentially make sure the 3D scene LOOKS 3D
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), width / height, 0.1f, 200.0f);
  shader->setMat4("projection", projection);
}

void ParticleEmitter::Start()
{
  lastUsedParticle = 0;

  emitterDuration = 0;


  if (!emitOverTime)
  {
    for (GLuint i = 0; i < particleSpawnRate; i++)
    {
      GLuint unusedParticle = FirstUnusedParticle();
      if (unusedParticle != -1)
        RespawnParticle(particles[unusedParticle]);
      else
        std::cout << "ERROR::PARTICLE_EMITTER::UPDATE" << std::endl << "Not caching enough particles" << std::endl;
    }
  }

  isPlaying = true;
}

void ParticleEmitter::Stop()
{
  isPlaying = false;

  for (Particle &particle : particles)
    particle.Life = 0;
}

void ParticleEmitter::Update(const GLfloat deltaTime)
{
  if (!isPlaying)
    return;

  emitterDuration += deltaTime;

  // Add new particles
  if (emitterDuration < 1.4f)
  {
    if (emitOverTime)
    {
      if (timeToNextParticle <= 0)
      {
        GLuint unusedParticle = FirstUnusedParticle();
        if (unusedParticle != -1)
          RespawnParticle(particles[unusedParticle], emitterDuration);
        else
          std::cout << "ERROR::PARTICLE_EMITTER::UPDATE" << std::endl << "Not caching enough particles" << std::endl;

        if (isSpinningTrail)
        {
          GLuint unusedParticle = FirstUnusedParticle();
          if (unusedParticle != -1)
            RespawnParticle(particles[unusedParticle], emitterDuration + M_PI);
          else
            std::cout << "ERROR::PARTICLE_EMITTER::UPDATE" << std::endl << "Not caching enough particles" << std::endl;

        }

        timeToNextParticle = 1.0f / particleSpawnRate;
      }
      else
        timeToNextParticle -= deltaTime;
    }
  }

  // Update all particles
  for (GLuint i = 0; i < nr_particles; i++)
  {
    Particle &p = particles[i];
    p.Life -= deltaTime;
    if (p.Life > 0.0f)
    {
      p.Velocity += gravity * deltaTime;
      p.Position += p.Velocity * deltaTime;

      //p.Size -= deltaTime * 0.05f;
      //p.Color.a -= deltaTime;
    }
  }
}

void ParticleEmitter::Draw() const
{
  if (!isPlaying)
    return;

  //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);

  for (Particle particle : particles)
  {
    if (particle.Life <= 0)
      continue;

    shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, particle.Position);

    shader->setVec4("color", particle.Color);
    GLfloat val = quadFunc(particle.Life, -1, -1.5f, 2);
    if (val > 1.5f)
    {
      model = glm::scale(model, particle.Size);
      shader->setMat4("model", model);
      shader->setFloat("mixAmount", val);
    }
    else
    {
      model = glm::scale(model, 0.8f * particle.Size);
      shader->setMat4("model", model);
      shader->setFloat("mixAmount", 0.5f);
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }
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
  return -1;
}

void ParticleEmitter::RespawnParticle(Particle &particle, const GLfloat angle)
{
  glm::vec3 dir;
  if (isSpinningTrail)
    dir = glm::vec3(cos(angle), 0.0f, sin(angle));
  else
  {
    do
    {
      dir = randomVec3(glm::vec3(-1.0f), glm::vec3(1.0f));
    } while (glm::length(dir) > 1);
  }
  particle.Position = origin + dir * sphereRadius;
  // particle.Position = origin + 4 * emitterDuration * glm::vec3(0.0f, 1.0f, 0.0f) + dir * sphereRadius;
  particle.Velocity = startSpeed * dir;

  particle.Size = startSize;
  particle.Color = glm::vec4(startColor, 1.0f);
  particle.Life = randomFloat(minStartLifetime, maxStartLifetime);//tartLifetime;
}



void ParticleEmitter::SetEmitterVariables(
  const glm::vec3 &origin, const GLfloat sphereRadius, const GLfloat particleSpawnRate, const GLboolean emitOverTime, const GLboolean isSpinningTrail,
  const glm::vec3 &startSize, const glm::vec3 &startColor, const GLfloat minStartLifetime, const GLfloat maxStartLifetime, const GLfloat startSpeed,
  const glm::vec3 &gravity)
{
  this->origin = origin;
  this->sphereRadius = sphereRadius;
  this->particleSpawnRate = particleSpawnRate;
  this->emitOverTime = emitOverTime;
  this->isSpinningTrail = isSpinningTrail;

  this->startSize = startSize;
  this->startColor = startColor;
  this->minStartLifetime = minStartLifetime;
  this->maxStartLifetime = maxStartLifetime;
  this->startSpeed = startSpeed;
  
  this->gravity = gravity;
}

void ParticleEmitter::SetOrigin(const glm::vec3 newOrigin)
{
  this->origin = newOrigin;
}

void ParticleEmitter::SetColor(const glm::vec3 newColor)
{
  this->startColor = newColor;
}

void ParticleEmitter::SetStartSpeed(const GLfloat newSpeed)
{
  this->startSpeed = newSpeed;
}