#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glad/glad.h>

bool readFile(const std::string &sourcePath, std::string &out);
GLint randomInt(const GLint min, const GLint max);
GLfloat randomFloat(const GLfloat min, const GLfloat max);
glm::vec2 randomVec2(const glm::vec2 &min, const glm::vec2 &max);
glm::vec3 randomVec3(const glm::vec3 &min, const glm::vec3 &max);
GLfloat quadFunc(const GLfloat x, const GLfloat a, const GLfloat b, const GLfloat c);