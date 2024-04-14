#include "uniform_buffer.hpp"

hyp::UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding) {
	glGenBuffers(1, &m_bufferId);
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_bufferId);
}

hyp::UniformBuffer::~UniformBuffer() {
	glDeleteBuffers(1, &m_bufferId);
}

hyp::Shared<hyp::UniformBuffer> hyp::UniformBuffer::create(uint32_t size, uint32_t binding) {
	return hyp::CreateRef<UniformBuffer>(size, binding);
}

void hyp::UniformBuffer::setData(const void* data, uint32_t size, uint32_t offset) {
	glBindBuffer(GL_UNIFORM_BUFFER, m_bufferId);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
