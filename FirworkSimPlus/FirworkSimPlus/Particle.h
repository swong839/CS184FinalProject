#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

struct Particle
{
  glm::vec3 Position, Velocity, Size;
  glm::vec4 Color;
  GLfloat Life;

  Particle() : Position(0.0f), Velocity(0.0f), Size(0.0f), Color(1.0f), Life(0.0f) { }
};