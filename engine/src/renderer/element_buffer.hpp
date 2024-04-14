#ifndef HYP_EBO_HPP
#define HYP_EBO_HPP

#include <glad/glad.h>
#include <cstdint>

namespace hyp {
	class ElementBuffer {
	public:
		ElementBuffer(uint32_t* indices, uint32_t count);
		~ElementBuffer();

		void bind();
		void unbind();

		uint32_t getCount() const { return m_count; }

	private:
		uint32_t m_rendererId;
		uint32_t m_count;
	};

	using IndexBuffer = ElementBuffer;
}

#endif // !HYP_EBO_HPP