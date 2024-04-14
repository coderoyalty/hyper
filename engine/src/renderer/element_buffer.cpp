#include "element_buffer.hpp"

hyp::ElementBuffer::ElementBuffer(uint32_t* indices, uint32_t count)
    : m_count(count) {
	glGenBuffers(1, &m_rendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}

hyp::ElementBuffer::~ElementBuffer() {
	glDeleteBuffers(1, &m_rendererId);
}

void hyp::ElementBuffer::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void hyp::ElementBuffer::unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
