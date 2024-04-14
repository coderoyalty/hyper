#ifndef HYPER_VERTEX_ARRAY_HPP
#define HYPER_VERTEX_ARRAY_HPP

#include <cstdint>
#include <vector>
#include <renderer/vertex_buffer.hpp>
#include <renderer/element_buffer.hpp>

namespace hyp {
	class VertexArray {
	public:
		VertexArray();

		~VertexArray();

	public:
		static hyp::Shared<VertexArray> create();

	public:
		void bind();
		void unbind();

		void addVertexBuffer(const Ref<VertexBuffer>& vbuffer);
		void setIndexBuffer(const Ref<hyp::ElementBuffer>& element_buffer);

		const std::vector<Ref<VertexBuffer>>& getVertexBuffers() const {
			return m_vbuffers;
		}

		const Ref<ElementBuffer>& getElementBuffer() const {
			return m_elementBuffer;
		}

	private:
		uint32_t m_rendererID;
		uint32_t m_vbufferIndex = 0;
		std::vector<Ref<VertexBuffer>> m_vbuffers;
		Ref<ElementBuffer> m_elementBuffer;
	};

};

#endif