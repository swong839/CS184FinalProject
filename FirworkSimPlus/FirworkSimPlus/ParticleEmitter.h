#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#include "stb_image.h"

#include "Particle.h"
#include "Shader.h"
#include "Utility.h"

class ParticleEmitter
{
public:
  ParticleEmitter(const GLuint nr_particles, const std::string vertexPath, const std::string fragmentPath);
  ~ParticleEmitter();
  void ConfigureShader(const float width, const float height);
  void Start();
  void Stop();
  void Update(const GLfloat deltaTime);
  void Draw() const;

  void SetEmitterVariables(
    const glm::vec3 &origin, const GLfloat sphereRadius, const GLfloat particleSpawnRate, const GLboolean emitOverTime, const GLboolean isSpinningTrail,
    const glm::vec3 &startSize, const glm::vec3 &startColor, const GLfloat minStartLifetime, const GLfloat maxStartLifetime, const GLfloat startSpeed,
    const glm::vec3 &gravity);

  void SetOrigin(const glm::vec3 newOrigin);
  void SetColor(const glm::vec3 newColor);
  void SetStartSpeed(const GLfloat newSpeed);

private:
  void SetupQuad();
  void SetupTexture();

  GLuint FirstUnusedParticle();
  void RespawnParticle(Particle &particle, const GLfloat angle = 0);

private:
  // Emitter object variables
  //-------------------------
  GLfloat emitterDuration;

  glm::vec3 origin;
  GLfloat sphereRadius;
  GLfloat particleSpawnRate;
  GLfloat timeToNextParticle;
  GLboolean emitOverTime;
  GLboolean isSpinningTrail;

  // Starting settings for particles
  glm::vec3 startSize;
  glm::vec3 startColor;
  GLfloat minStartLifetime;
  GLfloat maxStartLifetime;
  GLfloat startSpeed;
  
  // Settings for lifetime of particles
  glm::vec3 gravity;


  bool isPlaying;

  // Particle variables
  //-------------------
  GLuint lastUsedParticle;
  GLuint nr_particles;
  std::vector<Particle> particles;

  // OpenGL variables
  unsigned int VAO, VBO, EBO;
  unsigned int texture;
  Shader *shader;
};