#include "renderer/framebuffer.hpp"

namespace utils {
	void attachColorTexture(uint32_t texture, uint32_t width, uint32_t height, GLenum internalFormat, GLenum format, int index);
}

hyp::Framebuffer::Framebuffer(const hyp::FramebufferSpecification& spec) : m_spec(spec), m_fbo(0) {
	for (auto& attachment : spec.attachment.attachments)
	{
		m_colorAttachmentSpecs.emplace_back(attachment);
	}

	reset();
}

hyp::Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &m_fbo);
	glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
}

void hyp::Framebuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glViewport(0, 0, m_spec.width, m_spec.height);
}

void hyp::Framebuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

uint32_t hyp::Framebuffer::getColorAttachmentId(uint32_t index) {
	HYP_ASSERT_CORE(index < m_colorAttachments.size(), "attempt to access an invalid color attachments location");
	return m_colorAttachments[index];
}

void hyp::Framebuffer::resize(uint32_t width, uint32_t height) {
	if (width == 0 || height == 0 || width > hyp::MaxFramebufferSize || height > hyp::MaxFramebufferSize)
	{
		HYP_WARN("Attempt to resize framebuffer to %d %d is futile; max: %d", width, height, hyp::MaxFramebufferSize);
		return;
	}

	m_spec.width = width;
	m_spec.height = height;
	reset();
}

void hyp::Framebuffer::reset() {
	if (m_fbo)
	{
		glDeleteFramebuffers(1, &m_fbo);
		glDeleteTextures(m_colorAttachments.size(), m_colorAttachments.data());
		m_colorAttachments.clear();
	}

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	if (m_colorAttachmentSpecs.size())
	{
		m_colorAttachments.resize(m_colorAttachmentSpecs.size());

		// create textures
		glGenTextures(m_colorAttachments.size(), m_colorAttachments.data());

		for (size_t i = 0; i < m_colorAttachments.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, m_colorAttachments[i]);

			switch (m_colorAttachmentSpecs[i].textureFormat)
			{
			case hyp::FbTextureFormat::RGBA:
			{
				utils::attachColorTexture(m_colorAttachments[i],
				    m_spec.width, m_spec.height, GL_RGBA8, GL_RGBA, i);
				break;
			}
			default:
				HYP_ASSERT_CORE(false, "Frame buffer texture format is invalid");
				break;
			}
		}
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		HYP_WARN("Framebuffer is not complete!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/* Utils Function */

void utils::attachColorTexture(uint32_t texture, uint32_t width, uint32_t height, GLenum internalFormat, GLenum format, int index) {
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, texture, 0);
}
