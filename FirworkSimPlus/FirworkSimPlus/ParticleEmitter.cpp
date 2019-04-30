#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter(const GLuint nr_particles)
{
  this->nr_particles = nr_particles;
  for (GLuint i = 0; i < nr_particles; i++)
    particles.push_back(Particle());
}