#ifndef VERTEX_BUFFER_HPP
#define VERTEX_BUFFER_HPP
#include <glad/glad.h>
#include <cstdint>
#include "renderer/buffer.hpp"

namespace hyp
{
	class VertexBuffer
	{
	public:
		VertexBuffer(uint32_t size);
		VertexBuffer(float* vertices, uint32_t size);

		~VertexBuffer();

		void setData(void* vertices, uint32_t size);

		void bind();
		void unbind();

		const BufferLayout& getLayout() const
		{
			return m_layout;
		}
		void setLayout(const BufferLayout& layout)
		{
			m_layout = layout;
		};

	private:
		BufferLayout m_layout;
		uint32_t m_rendererId;
	};

};
#endif