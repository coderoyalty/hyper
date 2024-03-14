#pragma once
#ifndef HYPER_TEXTURE_HPP
#define HYPER_TEXTURE_HPP

#include <glad/glad.h>
#include <cstdint>
#include <string>
#include <system/export.hpp>
#include <stb_image.h>

namespace hyp {
	class HYPER_API Texture
	{
	public:
		Texture(const std::string& m_path);

		uint32_t getHeight() const {
			return m_height;
		}
		uint32_t getWidth() const {
			return m_width;
		}

		void bind(int8_t slot = 0);
		void unbind();
	private:
		uint32_t m_texture;
		std::string m_path;
		uint32_t m_width;
		uint32_t m_height;
		bool m_loaded;
	};
}

#endif

