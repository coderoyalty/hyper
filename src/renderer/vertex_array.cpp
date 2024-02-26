#include "vertex_array.hpp"
#include "renderer/buffer.hpp"
#include "assert.hpp"
#include "log.hpp"

namespace Utils
{
  static GLenum MapShaderDataTypeToOpenGL(ShaderDataType type)
  {
    switch (type)
    {
    case ShaderDataType::Float:
      return GL_FLOAT;
    case ShaderDataType::Vec2:
      return GL_FLOAT;
    case ShaderDataType::Vec3:
      return GL_FLOAT;
    case ShaderDataType::Vec4:
      return GL_FLOAT;
    case ShaderDataType::Mat3:
      return GL_FLOAT;
    case ShaderDataType::Mat4:
      return GL_FLOAT;
    case ShaderDataType::Int:
      return GL_INT;
    case ShaderDataType::IVec2:
      return GL_INT;
    case ShaderDataType::IVec3:
      return GL_INT;
    case ShaderDataType::IVec4:
      return GL_INT;
    case ShaderDataType::Bool:
      return GL_BOOL;
    }

    m_assert(false, "Unknown ShaderDataType!");
    return 0;
  };
}

VertexArray::VertexArray()
{
  glGenVertexArrays(1, &m_rendererID);
  this->bind();
}

VertexArray::~VertexArray()
{
  glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArray::bind()
{
  glBindVertexArray(m_rendererID);
}

void VertexArray::unbind()
{
  glBindVertexArray(0);
}

void VertexArray::addVertexBuffer(const Ref<VertexBuffer> &vbuffer)
{
  m_assert(vbuffer->getLayout().getAttributes().size() != 0, "vertex buffer has no layout");

  const auto &layout = vbuffer->getLayout();
  for (const auto &attribute : layout)
  {
    switch (attribute.type)
    {
    // int type attribute can't be normalized
    case ShaderDataType::Int:
    case ShaderDataType::IVec2:
    case ShaderDataType::IVec3:
    case ShaderDataType::IVec4:
    {
      glEnableVertexAttribArray(m_vbufferIndex);
      glVertexAttribIPointer(
          m_vbufferIndex, attribute.getComponentCount(),
          Utils::MapShaderDataTypeToOpenGL(attribute.type),
          layout.getStride(),
          (const void *)attribute.offset);
      m_vbufferIndex++;
      break;
    }
    case ShaderDataType::Float:
    case ShaderDataType::Vec2:
    case ShaderDataType::Vec3:
    case ShaderDataType::Vec4:
    {
      glEnableVertexAttribArray(m_vbufferIndex);
      glVertexAttribPointer(
          m_vbufferIndex, attribute.getComponentCount(),
          Utils::MapShaderDataTypeToOpenGL(attribute.type),
          attribute.normalized ? GL_TRUE : GL_FALSE,
          layout.getStride(),
          (const void *)attribute.offset);
      m_vbufferIndex++;
      break;
    }
    case ShaderDataType::Mat3: // same as vec3 * 3
    case ShaderDataType::Mat4: // same as vec4 * 4
    {
      uint32_t count = attribute.getComponentCount();

      for (uint32_t i = 0; i < count; i++)
      {
        glEnableVertexAttribArray(m_vbufferIndex);
        glVertexAttribPointer(
            m_vbufferIndex, count,
            Utils::MapShaderDataTypeToOpenGL(attribute.type),
            attribute.normalized ? GL_TRUE : GL_FALSE,
            layout.getStride(),
            (const void *)(attribute.offset + sizeof(float) * i * count));
        glVertexAttribDivisor(m_vbufferIndex, 1);
        m_vbufferIndex++;
      }
      break;
    }
    default:
      LOG_WARN("Unknown ShaderType");
    }
  }
  m_vbuffers.push_back(vbuffer);
}
