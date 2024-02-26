#include "vertex_buffer.hpp"

VertexBuffer::VertexBuffer(uint32_t size)
{
  glGenBuffers(1, &m_rendererId);
  this->bind();
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
  glDeleteBuffers(1, &m_rendererId);
}

VertexBuffer::VertexBuffer(float *vertices, uint32_t size)
{
  glGenBuffers(1, &m_rendererId);
  this->bind();
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VertexBuffer::setData(float *vertices, uint32_t size)
{
  this->bind();
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}

void VertexBuffer::bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
}

void VertexBuffer::unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
