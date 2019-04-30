#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
  // the program ID
  unsigned int ID;

  // constructor reads and builds the shader
  Shader(const std::string vertexPath, const std::string fragmentPath);
  // use/activate the shader
  void use();
  // utility uniform functions
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec2(const std::string &name, glm::vec2 value) const;
  void setVec4(const std::string &name, glm::vec4 value) const;
  void setMat4(const std::string &name, glm::mat4 value) const;
  void setTexture(const std::string &name, unsigned int value) const;
};

#endif