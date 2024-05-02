#pragma once

#ifndef HYP_FRAMEBUFFER_HPP

	#define HYP_FRAMEBUFFER_HPP
	#include <cstdint>
	#include <core/application.hpp>
	#include <vector>

namespace hyp {
	enum class FbTextureFormat
	{
		None = 0,
		RGBA,
		RED_INT,

		// Depth
		DEPTH24_STENCIL8
	};

	struct FbTextureSpecification
	{
		FbTextureSpecification() = default;
		FbTextureSpecification(FbTextureFormat format) : textureFormat(format) {
		}

		FbTextureFormat textureFormat = FbTextureFormat::None;
	};

	struct FbAttachmentSpecification
	{
		FbAttachmentSpecification() = default;
		FbAttachmentSpecification(std::initializer_list<FbTextureSpecification> attachments)
		    : attachments(attachments) {}

		std::vector<FbTextureSpecification> attachments;
	};

	struct FramebufferSpecification
	{
		uint32_t width, height;
		FbAttachmentSpecification attachment;
	};

	static const int MaxFramebufferSize = 1024 * 8;

	class Framebuffer {
	public:
		Framebuffer(const FramebufferSpecification& spec);
		~Framebuffer();

		static hyp::Ref<Framebuffer> create(const FramebufferSpecification& spec) {
			return hyp::CreateRef<hyp::Framebuffer>(spec);
		};

		void bind();
		void unbind();

		uint32_t getColorAttachmentId(uint32_t index = 0);

		void resize(uint32_t width, uint32_t height);

		void reset();

		const FramebufferSpecification& getSpecification() const {
			return m_spec;
		}

		int32_t readPixel(uint32_t attachmentId, int x, int y);

		void clearAttachment(uint32_t attachmentId, int value);
	private:
		uint32_t m_fbo;
		FramebufferSpecification m_spec;

		std::vector<FbTextureSpecification> m_colorAttachmentSpecs;
		std::vector<unsigned int> m_colorAttachments;

		FbTextureSpecification m_depthAttachmentSpec;
		uint32_t m_depthAttachment;
	};

}

#endif // !HYP_FRAMEBUFFER_HPP
