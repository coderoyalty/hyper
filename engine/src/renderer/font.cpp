#include "font.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

// (royalty) can this be set the before loading the font?
uint16_t hyp::Font::s_fontSize = 48;

hyp::Ref<hyp::Font> hyp::Font::create(const fs::path& fontFilePath) {
	return hyp::CreateRef<hyp::Font>(fontFilePath);
}

hyp::Ref<hyp::Font> hyp::Font::getDefault() {
	static hyp::Ref<hyp::Font> s_fontDefault;

	if (s_fontDefault)
	{
		return s_fontDefault;
	}

	s_fontDefault = hyp::Font::create("assets/fonts/CascadiaCode.ttf");
	HYP_INFO("Loaded engine's default font");
	return s_fontDefault;
}

hyp::Font::Font(const fs::path& fontFilePath) {
	const int textureWidth = 512;
	const int textureHeight = 512;
	char* textureBuffer = new char[textureWidth * textureHeight];
	m_fontGeometry = hyp::CreateRef<hyp::FontGeometry>();

	FT_Library ft;

	// initialize FreeType library
	HYP_ASSERT_CORE(FT_Init_FreeType(&ft) == 0, "Couldn't initialize FreeType library");

	HYP_ASSERT_CORE(fs::exists(fontFilePath), "Font file does not exists");

	FT_Face fontFace;
	HYP_ASSERT_CORE(FT_New_Face(ft, fontFilePath.string().c_str(), 0, &fontFace) == 0,
	    "Couldn't load font");

	FT_Set_Pixel_Sizes(fontFace, 0, s_fontSize);

	int padding = 2;
	int row = 0;
	int col = padding;
	unsigned int fontHeight = 0;

	FontMetrics metrics {};
	metrics.ascender = fontFace->ascender;
	metrics.descender = fontFace->descender;
	int ascent = metrics.ascender >> 6;
	int descent = -metrics.descender >> 6;
	metrics.lineHeight = ascent + descent;

	m_fontGeometry->metrics = metrics;

	// iterate through ASCII characters from 32 to 127
	for (int ch = 32; ch < 128; ch++)
	{
		// load each character
		if (FT_Load_Char(fontFace, ch, FT_LOAD_RENDER))
		{
			HYP_WARN("Failed to load glyph %c", ch);
			continue;
		}

		FT_GlyphSlot g = fontFace->glyph;
		FT_Bitmap bitmap = g->bitmap;

		// check if current character exceeds texture width
		if (col + bitmap.width + padding >= textureWidth)
		{
			col = padding;
			row += s_fontSize; // this automatically supports row padding (as not all character will be == s_fontSize)
		}

		fontHeight = std::max(fontHeight, bitmap.rows);

		// copy each character's bitmap value to the texture buffer
		for (unsigned int y = 0; y < bitmap.rows; ++y)
		{
			for (unsigned int x = 0; x < bitmap.width; ++x)
			{
				// copy bitmap value to textureBuffer starting from (row,col) to (row+bitmap.rows, col+bitmap.width)
				textureBuffer[(row + y) * textureWidth + (col + x)] =
				    bitmap.buffer[y * bitmap.width + x];
			}
		}

		Glyph glyph {};
		glyph.advance = { g->advance.x >> 6, g->advance.y >> 6 };
		glyph.size = { bitmap.width, bitmap.rows };
		glyph.offset = { g->bitmap_left, g->bitmap_top };
		glyph.uvCoords = { col, row };

		m_fontGeometry->glyphs[ch] = glyph;
		// Update column position for next character
		col += bitmap.width + padding;
	}

	// unload font face and library
	FT_Done_Face(fontFace);
	FT_Done_FreeType(ft);

	// create and populate texture with the texture buffer data
	{
		hyp::TextureSpecification texSpec;
		texSpec.format = hyp::TextureFormat::RED;
		texSpec.width = textureWidth;
		texSpec.height = textureHeight;
		texSpec.mipmap = false;
		m_texture = hyp::Texture2D::create(texSpec);
		m_texture->setData(textureBuffer, textureWidth * textureHeight);
	}

	delete[] textureBuffer;
}

void hyp::Glyph::getQuadAtlasBounds(glm::vec2& min, glm::vec2& max) const {
	min = this->uvCoords;
	max = min + this->size;
}

void hyp::Glyph::getQuadPlaneBounds(glm::vec2& plane_min, glm::vec2& plane_max, float scale) const {
	if (size.x > 0 && size.y > 0)
	{
		float x = offset.x * scale;
		float y = -offset.y * scale;

		float w = size.x * scale;
		float h = size.y * scale;

		plane_min.x = x;
		plane_min.y = y;

		plane_max.x = x + w;
		plane_max.y = y + h;
	}
	else
	{
		plane_min.x = 0, plane_min.y = 0, plane_max.x = 0, plane_max.y = 0;
	}
}

hyp::Glyph* hyp::FontGeometry::getGlyph(char ch) {
	if (glyphs.find(ch) == glyphs.end()) return nullptr;
	return &glyphs[ch];
}

double hyp::FontGeometry::getAdvance(char ch, char nextChar) {
	auto glyph1 = getGlyph(ch);
	auto glyph2 = getGlyph(nextChar);

	return glyph2->offset.x - (glyph1->offset.x + glyph1->size.x);
}
