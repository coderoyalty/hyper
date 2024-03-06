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

  class Shader
  {
  public:
    Shader(SHADER_TYPE type);
    ~Shader();
    bool compile(std::string shaderSource);
    unsigned int getShader()
    {
      return this->shader;
    }

    SHADER_TYPE getType()
    {
      return this->shader_type;
    }

    bool operator==(const Shader &shader);

  private:
    friend class ShaderProgram;

  protected:
    const std::string id;
    SHADER_TYPE shader_type;
    unsigned int shader;

  private:
    bool m_isCompiled = false;
  };

  class ShaderProgram
  {
  public:
    ShaderProgram(const std::string &vertexPath, const std::string &fragmentPath);
    ShaderProgram();
    void attachShader(Shader &shader);
    void link();
    void use();

  private:
    bool m_isLinked;
    std::vector<Ref<Shader>> m_attachedShaders;
    unsigned int m_program;
  };
};
#endif