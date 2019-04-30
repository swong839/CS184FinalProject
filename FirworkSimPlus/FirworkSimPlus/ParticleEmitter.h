#pragma once

#include <glad/glad.h>

#include <vector>

#include "Particle.h"

class ParticleEmitter
{
public:
  ParticleEmitter(const GLuint nr_particles = 500);

private:
  GLuint nr_particles;
  std::vector<Particle> particles;
};