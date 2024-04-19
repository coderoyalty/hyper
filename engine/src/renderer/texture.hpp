#pragma once
#ifndef HYPER_TEXTURE_HPP
	#define HYPER_TEXTURE_HPP

	#include <glad/glad.h>
	#include <cstdint>
	#include <string>
	#include <system/export.hpp>
	#include <stb_image.h>
	#include <core/base.hpp>

namespace hyp {

	enum class TextureFormat
	{
		None,
		RED,
		RGB,
		RGBA,
		RGBA32F,
	};

	struct TextureSpecification
	{
		TextureFormat format = hyp::TextureFormat::RGBA;
		uint32_t width = 1;
		uint32_t height = 1;
		bool mipmap = true;
	};

	class HYPER_API Texture {
	public:
		Texture(const TextureSpecification& spec);
		Texture(const std::string& path);

		~Texture();

	public:
		static hyp::Ref<Texture> create(const TextureSpecification& spec);
		static hyp::Ref<Texture> create(const std::string& path);

	public:
		uint32_t getWidth() const {
			return m_width;
		}

		uint32_t getHeight() const {
			return m_height;
		}

		uint32_t getTextureId() const {
			return m_texture;
		}

		bool operator==(const Texture& other) const {
			return m_texture == other.getTextureId();
		}

		void bind(int8_t slot = 0);

		void unbind();

		void setData(void* data, uint32_t size);
		std::string getPath() const {
			return m_path;
		}

		bool isLoaded() const {
			return m_loaded;
		}

	private:
		TextureSpecification m_spec;

		uint32_t m_texture;
		uint32_t m_width, m_height;
		std::string m_path;
		bool m_loaded = false;

		unsigned int m_internalFormat, m_dataFormat;
	};

	/*
	* @brief less vague meaning of the Texture class
	*/
	using Texture2D = Texture;
}

#endif
