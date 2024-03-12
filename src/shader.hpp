#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include "core/base.hpp"
namespace hyp
{

  enum SHADER_TYPE
  {
    FRAGMENT = GL_FRAGMENT_SHADER,
    VERTEX
  };

  class ShaderProgram
  {
  public:
    ShaderProgram(const std::string &vertexPath, const std::string &fragmentPath);
    ShaderProgram();
    void link();
    void use();

  private:
    void attachShader(uint32_t &shader);
    bool m_isLinked = false;
    unsigned int m_program;
  };
};
#endif