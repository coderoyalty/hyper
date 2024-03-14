#include "texture.hpp"
#include <utils/logger.hpp>
#include <utils/assert.hpp>

hyp::Texture::Texture(const std::string& path) : m_path(path), m_texture(0)
{

	int width, height, nrchannels;
	unsigned char* pixels;

	pixels = stbi_load(path.c_str(), &width, &height, &nrchannels, 0);

	m_width = width;
	m_height = height;

	int dataformat = 0, internalformat = 0;

	HYP_ASSERT_CORE(pixels != nullptr, "Couldn't load image file");

	m_loaded = true;
	if (nrchannels == 4) {
		dataformat = internalformat = GL_RGBA;
	}
	else if (nrchannels == 3) {
		dataformat = internalformat = GL_RGB;
	}

	if (!(internalformat & dataformat))
	{
		HYP_ERROR("Image format not supported");
		stbi_image_free(pixels);
		return;
	}

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, dataformat, GL_UNSIGNED_BYTE, pixels);

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(pixels);
}

void hyp::Texture::bind(int8_t slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void hyp::Texture::unbind() {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}