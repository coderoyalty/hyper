#ifndef VAO_HPP
#define VAO_HPP

#include <cstdint>
#include <vector>
#include "renderer/vertex_buffer.hpp"

class VertexArray
{
public:
  VertexArray();

  ~VertexArray();

  void bind();
  void unbind();

  void addVertexBuffer(const Ref<VertexBuffer> &vbuffer);

  const std::vector<Ref<VertexBuffer>> &getVertexBuffers() const
  {
    return m_vbuffers;
  }

private:
  uint32_t m_rendererID;
  uint32_t m_vbufferIndex = 0;
  std::vector<Ref<VertexBuffer>> m_vbuffers;
};

#endif