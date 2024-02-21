#include "shader.hpp"
#include "utils.hpp"

Shader::Shader(SHADER_TYPE type) : id(generateUUID())
{
  this->shader_type = type;
  this->shader = glCreateShader(type);
}

Shader::~Shader()
{
  glDeleteShader(shader);
}

bool Shader::compile(const char *shaderSource)
{
  if (m_isCompiled)
  {
    return false;
  }

  glShaderSource(this->shader, 1, &shaderSource, nullptr);
  glCompileShader(this->shader);

  this->m_isCompiled = true;
  return true;
}

bool Shader::operator==(Shader &shader)
{
  if (shader.id == this->id)
  {
    return true;
  }
  return false;
}

ShaderProgram::ShaderProgram()
{
  this->m_program = glCreateProgram();
}

void ShaderProgram::attachShader(Shader *shader)
{
  if (this->m_isLinked)
  {
    return;
  }
  unsigned int raw_shader = shader->getShader();
  for (auto it = m_attachedShaders.begin(); it < m_attachedShaders.end(); it++)
  {
    if (shader == (*it))
    {
      return;
    }
  }

  glAttachShader(m_program, raw_shader);

  m_attachedShaders.push_back(shader);
}

void ShaderProgram::link()
{
  if (this->m_isLinked)
  {
    return;
  }
  glLinkProgram(this->m_program);
  this->m_isLinked = true;
}

void ShaderProgram::use()
{
  glUseProgram(this->m_program);
}
