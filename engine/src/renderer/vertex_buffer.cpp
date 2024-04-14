#include "vertex_buffer.hpp"

namespace hyp {

	VertexBuffer::VertexBuffer(uint32_t size) {
		glGenBuffers(1, &m_rendererId);
		this->bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::~VertexBuffer() {
		glDeleteBuffers(1, &m_rendererId);
	}

	hyp::Shared<hyp::VertexBuffer> VertexBuffer::create(uint32_t size) {
		return hyp::CreateRef<hyp::VertexBuffer>(size);
	}

	hyp::Shared<hyp::VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size) {
		return hyp::CreateRef<hyp::VertexBuffer>(vertices, size);
	}

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size) {
		glGenBuffers(1, &m_rendererId);
		this->bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	void VertexBuffer::setData(void* vertices, uint32_t size) {
		this->bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
	}

	void VertexBuffer::bind() {
		glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
	}

	void VertexBuffer::unbind() {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
} // namespace hyp
