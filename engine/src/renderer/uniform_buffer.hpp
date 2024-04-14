#pragma once
#ifndef HYP_UNIFORM_BUFFER_HPP
	#define HYP_UNIFORM_BUFFER_HPP

	#include <glad/glad.h>
	#include <core/base.hpp>
	#include <cstdint>

namespace hyp {
	class UniformBuffer {
	public:
		UniformBuffer(uint32_t size, uint32_t binding);

		~UniformBuffer();

	public:
		static hyp::Shared<UniformBuffer> create(uint32_t size, uint32_t binding);

	public:
		void setData(const void* data, uint32_t size, uint32_t offset = 0);

	private:
		uint32_t m_bufferId;
	};
}

#endif