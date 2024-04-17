#include "texture.hpp"
#include <utils/logger.hpp>
#include <utils/assert.hpp>

namespace utils {
	static uint32_t toGlFormat(const hyp::TextureFormat& format) {
		switch (format)
		{
		case hyp::TextureFormat::RGB:
			return GL_RGB8;
		case hyp::TextureFormat::RGBA:
			return GL_RGBA8;
		case hyp::TextureFormat::RED:
			return GL_R8;
		}

		return 0;
	}

	static GLenum toGlDataFormat(const hyp::TextureFormat& format) {
		switch (format)
		{
		case hyp::TextureFormat::RGB:
			return GL_RGB;
		case hyp::TextureFormat::RGBA:
			return GL_RGBA;
		case hyp::TextureFormat::RED:
			return GL_RED;
		}

		return 0;
	}

	static uint32_t toGlFormatSize(GLenum format) {
		switch (format)
		{
		case GL_RGB:
			return 3;
		case GL_RGBA:
			return 4;
		case GL_RED:
			return 1;
		default:
			break;
		}

		HYP_ASSERT(false);
		return 0;
	}
}

hyp::Texture::Texture(const TextureSpecification& spec) {
	m_spec = spec;

	m_width = spec.width;
	m_height = spec.height;

	m_internalFormat = utils::toGlFormat(spec.format);
	m_dataFormat = utils::toGlDataFormat(spec.format);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, nullptr);
}

hyp::Texture::Texture(const std::string& path)
    : m_path(path), m_texture(0),
      m_internalFormat(0), m_dataFormat(0) {
	int width, height, channels;
	unsigned char* pixels;

	stbi_set_flip_vertically_on_load(1);
	pixels = stbi_load(path.c_str(), &width, &height, &channels, 0);

	m_width = width;
	m_height = height;

	m_spec.height = height;
	m_spec.width = width;
	m_spec.mipmap = true;

	int dataFormat = 0, internalFormat = 0;

	if (!pixels)
	{
		return;
	}

	m_loaded = true;

	if (channels == 4)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (channels == 3)
	{
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	m_internalFormat = internalFormat;
	m_dataFormat = dataFormat;

	if (!(dataFormat & internalFormat))
	{
		HYP_ERROR("Image format not supported");
		stbi_image_free(pixels);
		m_loaded = false;
		return;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	m_loaded = true;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, pixels);

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(pixels);
}

hyp::Texture::~Texture() {
	glDeleteTextures(1, &m_texture);
}

hyp::Ref<hyp::Texture> hyp::Texture::create(const hyp::TextureSpecification& spec) {
	return hyp::CreateRef<hyp::Texture>(spec);
}

hyp::Ref<hyp::Texture> hyp::Texture::create(const std::string& path) {
	return hyp::CreateRef<hyp::Texture>(path);
}

void hyp::Texture::bind(int8_t slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void hyp::Texture::unbind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void hyp::Texture::setData(void* pixels, uint32_t size) {
	uint32_t formatSize = utils::toGlFormatSize(m_dataFormat);

	HYP_ASSERT_CORE(m_width * m_height * formatSize == size, "data provided must be the entire pixels data");

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, pixels);
	if (m_spec.mipmap)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	m_loaded = true;
}