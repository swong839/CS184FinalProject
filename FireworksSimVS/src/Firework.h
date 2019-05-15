#pragma once

#include "ParticleEmitter.h"

enum ExplosionType
{
  DEFAULT_ET
};

enum TrailType
{
  DEFAULT_TT
};

enum SmokeType
{
  DEFAULT_ST
};

class Firework
{
public:
  Firework(const GLfloat gravity, const glm::vec3 &windForce,
    const ExplosionType explosionType, const std::string explosionVertexShaderPath, const std::string explosionFragmentShaderPath, GLfloat explosionAverageLifetime,
    const TrailType trailType, const std::string trailVertexShaderPath, const std::string trailFragmentShaderPath, GLfloat averageLifetime,
    const SmokeType smokeType, const std::string smokeVertexShaderPath, const std::string smokeFragmentShaderPath
    );
  ~Firework();
  void ConfigureShaders(const float width, const float height);

  void Start(const glm::vec3 &origin, const glm::vec3 &initialVelocity,
	  const glm::vec3 currentExplosionColor = glm::vec3(0), bool changeExplosionColor = false,
	  const GLfloat currentExplosionForce = 0, bool changeExplosionForce = false,
	  glm::vec3 windForces = glm::vec3(-10, 0, 0), GLfloat gravity = 9.8f, bool randomColor = false);
  void Update(const GLfloat deltaTime);
  void Draw();
  void Stop();

private:
  void CreateParticleEmitters(
    const ExplosionType explosionType, const std::string explosionVertexShaderPath, const std::string explosionFragmentShaderPath, GLfloat explosionAverageLifetime,
    const TrailType trailType, const std::string trailVertexShaderPath, const std::string trailFragmentShaderPath, GLfloat averageLifetime,
    const SmokeType smokeType, const std::string smokeVertexShaderPath, const std::string smokeFragmentShaderPath);

  void CreateDefaultExplosion(const std::string explosionVertexShaderPath, const std::string explosionFragmentShaderPath, GLfloat averageLifetime);
  void CreateDefaultTrail(const std::string trailVertexShaderPath, const std::string trailFragmentShaderPath, GLfloat averageLifetime);
  void CreateDefaultSmoke(const std::string smokeVertexShaderPath, const std::string smokeFragmentShaderPath);

private:
  GLfloat duration;

  glm::vec3 mainBodyPos;
  glm::vec3 externalForces;
  glm::vec3 velocity;

  GLboolean explosionStarted;
  ParticleEmitter *explosion;
  ParticleEmitter *trail;
  ParticleEmitter *smoke;

  GLboolean simulationRunning;
};