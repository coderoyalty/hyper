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
		void getQuadAtlasBounds(glm::vec2& atlas_min, glm::vec2& atlas_max) const;
		void getQuadPlaneBounds(glm::vec2& plane_min, glm::vec2& plane_max, float scale = 1.f) const;
		//NOTE: returning only x advance for now, as it only supports ASCII chars (meaning y will always be zero)
		double getAdvance() const {
			// thought I was going to do some sort of calculation... (might still)
			return advance.x;
		}

		glm::vec2 size;
		glm::vec2 offset;
		glm::vec2 advance;
		glm::vec2 uvCoords;
	};

	struct FontMetrics
	{
		int ascender;
		int descender;
		int lineHeight;
	};

	struct FontGeometry
	{
		hyp::Glyph* getGlyph(char ch);
		const FontMetrics& getMetrics() const {
			return metrics;
		}

		double getAdvance(char ch, char nextChar);
		FontMetrics metrics;
		std::map<char, Glyph> glyphs;
	};

	class Font {
	public:
		Font(const fs::path& fontFilePath);

		hyp::Ref<hyp::Texture2D> getAtlasTexture() const { return m_texture; }
		hyp::Ref<hyp::FontGeometry> getFontData() const { return m_fontGeometry; }

		static hyp::Ref<hyp::Font> getDefault();
		static hyp::Ref<hyp::Font> create(const fs::path& fontFilePath);
	private:
		// for caching text rendering information
		hyp::Ref<hyp::FontGeometry> m_fontGeometry;
		hyp::Ref<hyp::Texture2D> m_texture;
		static uint16_t s_fontSize;
	};
}

#endif