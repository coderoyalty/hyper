#pragma once
#ifndef HYP_FONT_HPP
	#define HYP_FONT_HPP
	#include <utils/assert.hpp>
	#include <utils/logger.hpp>
	#include <core/base.hpp>
	#include <cstddef>
	#include <renderer/texture.hpp>
	#include <filesystem>
	#include <glm/glm.hpp>
	#include <map>

namespace fs = std::filesystem;

namespace hyp {

	struct Glyph
	{
		glm::vec2 size;
		glm::vec2 offset;
		glm::vec2 advance;
		glm::vec2 uvCoords;

		void getQuadAtlasBounds(glm::vec2& atlas_min, glm::vec2& atlas_max) const;
		void getQuadPlaneBounds(glm::vec2& plane_min, glm::vec2& plane_max) const;
	};

	struct FontMetrics
	{
		int ascender;
		int descender;
		int lineHeight;
	};

	struct FontGeometry
	{
		FontMetrics metrics;
		std::map<char, Glyph> glyphs;
		hyp::Glyph getGlyph(char ch);
	};

	class Font {
	public:
		Font(const fs::path& fontFilePath);

		hyp::Ref<hyp::Texture2D> getAtlasTexture() const { return m_texture; }
		hyp::Ref<hyp::FontGeometry> getFontData() const { return m_fontGeometry; }

		//TODO: add default font to assets for all platform
	#ifdef _WIN32
		static hyp::Ref<hyp::Font> getDefault();
	#endif
	private:
		// for caching text rendering information
		hyp::Ref<hyp::FontGeometry> m_fontGeometry;
		hyp::Ref<hyp::Texture2D> m_texture;
		static uint16_t s_fontSize;
	};
}

#endif