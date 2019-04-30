#pragma once

#include <glad/glad.h>

#include <string>
#include <vector>

#include "stb_image.h"

#include "Particle.h"
#include "Shader.h"

class ParticleEmitter
{
public:
  ParticleEmitter(const GLuint nr_particles, const std::string vertexPath, const std::string fragmentPath);
  ~ParticleEmitter();
  void ConfigureShader(float width, float height);
  void Update(const GLfloat deltaTime, const GLuint nr_new_particles);
  void Draw() const;

private:
  void SetupQuad();
  void SetupTexture();

  GLuint FirstUnusedParticle();
  void RespawnParticle(Particle &particle, glm::vec2 initialPos, glm::vec2 offset, glm::vec2 initialVelocity);

private:
  GLuint lastUsedParticle;
  GLuint nr_particles;
  std::vector<Particle> particles;

  unsigned int VAO, VBO, EBO;
  unsigned int texture;
  Shader *shader;
};