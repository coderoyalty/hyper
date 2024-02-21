#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <vector>

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
  bool compile(const char *shaderSource);
  unsigned int getShader()
  {
    return this->shader;
  }

  SHADER_TYPE getType()
  {
    return this->shader_type;
  }

  bool operator==(Shader &shader);
  const std::string id;

protected:
  SHADER_TYPE shader_type;
  unsigned int shader;

private:
  bool m_isCompiled = false;
};

class ShaderProgram
{
public:
  ShaderProgram();
  void attachShader(Shader *shader);
  void link();
  void use();

private:
  bool m_isLinked;
  std::vector<Shader *> m_attachedShaders;
  unsigned int m_program;
};

#endif