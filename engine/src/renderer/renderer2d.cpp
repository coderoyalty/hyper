#define RENDERER_2D_DATA_STRUCTURES
#include <renderer/renderer2d.hpp>
#include <array>

using namespace hyp;

static RendererData s_renderer;

void hyp::Renderer2D::init() {
	HYP_INFO("Initialize 2D Renderer");

	utils::initQuad();
	utils::initLine();
	utils::initCircle();
	utils::initText();

	s_renderer.cameraUniformBuffer = hyp::UniformBuffer::create(sizeof(RendererData::CameraData), 0);
	s_renderer.lighting.uniformBuffer = hyp::UniformBuffer::create(sizeof(Light) * MaxLight, 2);
}

void Renderer2D::deinit() {
	s_renderer.quad.reset();
	s_renderer.line.reset();
	s_renderer.circle.reset();
	HYP_INFO("Destroyed 2D Renderer");
}

/*
* flushes all the entity batch (e.g quad, line etc.) data
*/

void Renderer2D::flush() {
	utils::flushQuad();
	utils::flushLine();
	utils::flushCircle();
	utils::flushText();
}

void Renderer2D::startBatch() {
	s_renderer.quad.reset();
	s_renderer.line.reset();
	s_renderer.circle.reset();
	s_renderer.text.reset();

	s_renderer.stats.drawCalls = 0;
	s_renderer.stats.lineCount = 0;
	s_renderer.stats.quadCount = 0;

	// lightings
	s_renderer.lighting.lights.clear();
	s_renderer.lighting.lightCount = 0;
}

/*
* flushes all the entity batch (e.g quad, line etc.) data, then starts a new one.
*/
void Renderer2D::nextBatch() {
	flush();
	startBatch();
}

void Renderer2D::beginScene(const glm::mat4& viewProjectionMatrix) {
	startBatch();

	s_renderer.cameraBuffer.viewProjection = viewProjectionMatrix;
	s_renderer.cameraUniformBuffer->setData(&s_renderer.cameraBuffer, sizeof(RendererData::CameraData));
}

void Renderer2D::endScene() {
	flush();
}

void Renderer2D::enableLighting(bool value) {
	s_renderer.lighting.enabled = value;
};

void Renderer2D::addLight(const Light& light) {
	auto& lighting = s_renderer.lighting;

	if (lighting.lightCount >= MaxLight) return;

	lighting.lights.push_back(light);
	lighting.lightCount++;
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position + glm::vec3(size / 2.f, 0.f));
	model = glm::scale(model, glm::vec3(size, 0.f));
	drawQuad(model, color);
}

void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, hyp::Ref<hyp::Texture2D> texture, float tilingFactor, const glm::vec4& color) {
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position + glm::vec3(size / 2.f, 0.f));
	model = glm::scale(model, glm::vec3(size, 0.f));

	drawQuad(model, texture, tilingFactor, color);
}

void Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color) {
	auto& quad = s_renderer.quad;

	if (quad.transforms.size() == MaxQuad)
	{
		// we've exceeded the Maximum batch for a quad at the point,
		// so we have to push it..
		utils::nextQuadBatch();
	}

	int quadVertexCount = 4;

	for (int i = 0; i < quadVertexCount; i++)
	{
		QuadVertex vertex;
		vertex.pos = quad.vertexPos[i];
		vertex.color = color;
		vertex.uv = quad.uvCoords[i];
		vertex.textureIndex = 0.0; // use default texture
		vertex.transformIndex = quad.transformIndexCount;
		vertex.tilingFactor = 1.f;

		quad.vertices.push_back(vertex);
	}

	quad.transforms.push_back(transform);
	quad.transformIndexCount++;
	s_renderer.quad.indexCount += 6;
}

/*
* @brief for rendering textured-quad
*/
void hyp::Renderer2D::drawQuad(const glm::mat4& transform, hyp::Ref<hyp::Texture2D>& texture, float tilingFactor, const glm::vec4& color) {
	auto& quad = s_renderer.quad;

	float textureIndex = 0.0;
	// find texture in slots
	for (uint32_t i = 1; i < quad.textureSlotIndex; i++)
	{
		if (*quad.textureSlots[i] == *texture)
		{
			textureIndex = (float)i;
			break;
		}
	}

	if (textureIndex == 0.0)
	{
		// texture is a new texture
		if (quad.textureSlotIndex == MaxTextureSlots)
			utils::nextQuadBatch(); // dispatch the current batch

		/// the texture slot index will never be = to MaxTextureSlots
		/// this logic above avoids this scenario, and is presumed to reset the slot index
		HYP_ASSERT_CORE(quad.textureSlotIndex != MaxTextureSlots, "texture slot limits exceeded");
		quad.textureSlots[quad.textureSlotIndex] = texture;
		textureIndex = (float)quad.textureSlotIndex++;
	}

	int quadVertexCount = 4;

	for (int i = 0; i < quadVertexCount; i++)
	{
		QuadVertex vertex;
		vertex.pos = quad.vertexPos[i];
		vertex.color = color;
		vertex.uv = quad.uvCoords[i];
		vertex.textureIndex = textureIndex;
		vertex.transformIndex = quad.transformIndexCount;
		vertex.tilingFactor = tilingFactor;

		quad.vertices.push_back(vertex);
	}

	quad.transforms.push_back(transform);
	quad.transformIndexCount++;
	s_renderer.quad.indexCount += 6;
}

void Renderer2D::drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color) {
	LineVertex v0, v1;

	v0.position = p1;
	v1.position = p2;

	v0.color = v1.color = color;

	s_renderer.line.vertices.push_back(v0);
	s_renderer.line.vertices.push_back(v1);

	if (s_renderer.line.vertices.size() == MaxQuad)
		utils::nextLineBatch();
}

void Renderer2D::drawCircle(const glm::mat4& transform, float thickness, float fade, const glm::vec4& color) {
	if (s_renderer.circle.vertices.size() == static_cast<unsigned long long>(MaxCircles) * 4)
	{
		utils::nextCircleBatch();
	}

	for (size_t i = 0; i < 4; i++)
	{
		CircleVertex vertex {};
		vertex.color = color;
		vertex.worldPosition = transform * s_renderer.quad.vertexPos[i]; // TODO: create circle's vertexPos to avoid coupled code
		vertex.localPosition = s_renderer.quad.vertexPos[i] * 2.f;
		vertex.thickness = thickness;
		vertex.fade = fade;

		s_renderer.circle.vertices.push_back(vertex);
	}

	s_renderer.circle.indexCount += 6;
}

void hyp::Renderer2D::drawString(const std::string& str, hyp::Ref<hyp::Font> font, const glm::mat4& transform, const TextParams& textParams) {
	auto& text = s_renderer.text;

	// switch to engine's default font, if the provided font is invalid
	if (!font)
	{
		font = hyp::Font::getDefault();
	}

	const auto& fontGeometry = font->getFontData();
	const auto& metrics = fontGeometry->getMetrics();
	auto fontAtlas = font->getAtlasTexture();
	text.fontAtlasTexture = fontAtlas;

	float x = 0.0;
	float y = 0.0;

	float scale = textParams.fontSize;
	float fontScalingFactor = 1.f / (metrics.ascender - metrics.descender);

	for (size_t i = 0; i < str.length(); i++)
	{
		char ch = str[i];

		// no need to create quads for whitespace characters
		if (ch == '\r') continue;
		if (ch == '\n')
		{
			x = 0.0;
			y += fontScalingFactor * scale * metrics.lineHeight + textParams.leading;
			continue;
		}

		if (ch == ' ')
		{
			float advance = (float)fontGeometry->getGlyph(' ')->getAdvance();
			x += fontScalingFactor * advance * scale;
			continue;
		}

		if (ch == '\t') {
			float advance = (float)fontGeometry->getGlyph(' ')->getAdvance();
			x += fontScalingFactor * advance * scale * 2.f;
			continue;
		}

		auto glyph = fontGeometry->getGlyph(ch);

		// avoid accessing a nullptr
		if (!glyph)
			glyph = fontGeometry->getGlyph((uint8_t)127);
		if (!glyph) continue;

		glm::vec2 uvMin, uvMax;
		glyph->getQuadAtlasBounds(uvMin, uvMax);

		glm::vec2 quadMin, quadMax;
		glyph->getQuadPlaneBounds(quadMin, quadMax, scale);

		float texelWidth = 1.f / fontAtlas->getWidth();
		float texelHeight = 1.f / fontAtlas->getHeight();

		uvMin *= glm::vec2(texelWidth, texelHeight);
		uvMax *= glm::vec2(texelWidth, texelHeight);

		quadMin *= fontScalingFactor;
		quadMax *= fontScalingFactor;
		quadMin += glm::vec2(x, y);
		quadMax += glm::vec2(x, y);

		TextVertex v0, v1, v2, v3;

		v0.position = transform * glm ::vec4(quadMin, 0.f, 1.f);
		v0.color = textParams.color;
		v0.uvCoord = uvMin;

		v1.position = transform * glm ::vec4(quadMin.x, quadMax.y, 0.f, 1.f);
		v1.color = textParams.color;
		v1.uvCoord = { uvMin.x, uvMax.y };

		v2.position = transform * glm ::vec4(quadMax, 0.f, 1.f);
		v2.color = textParams.color;
		v2.uvCoord = uvMax;

		v3.position = transform * glm ::vec4(quadMax.x, quadMin.y, 0.f, 1.f);
		v3.color = textParams.color;
		v3.uvCoord = { uvMax.x, uvMin.y };

		text.vertices.push_back(v0);
		text.vertices.push_back(v1);
		text.vertices.push_back(v2);
		text.vertices.push_back(v3);

		text.indexCount += 6;

		x += fontScalingFactor * glyph->advance.x * scale;
	}
}

/*Quad Data*/

void utils::initQuad() {
	auto& quad = s_renderer.quad;

	// initialize texture slots
	quad.defaultTexture = hyp::Texture2D::create(TextureSpecification());
	uint32_t whiteColor = 0xFFffFFff;
	quad.defaultTexture->setData(&whiteColor, sizeof(uint32_t));
	quad.textureSlots[0] = quad.defaultTexture;

	quad.vao = hyp::VertexArray::create();
	quad.vbo = hyp::VertexBuffer::create(MaxVertices * sizeof(QuadVertex));

	quad.vbo->setLayout({
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec3, "aPos", false),
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec4, "aColor", false),
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec2, "aUV", false),
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Int, "aTransformIndex", false),
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Float, "aTextureIndex", false),
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Float, "aTilingFactor", false),
	});

	quad.vao->addVertexBuffer(quad.vbo);
	quad.vertices.clear();
	quad.vertices.resize(MaxVertices);

	uint32_t* quadIndices = new uint32_t[MaxIndices];
	int offset = 0;
	int i = 0;
	for (i = 0; i < MaxIndices; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}
	hyp::Ref<hyp::ElementBuffer> elementBuffer = hyp::CreateRef<hyp::ElementBuffer>(quadIndices, MaxIndices);
	quad.vao->setIndexBuffer(elementBuffer);
	delete[] quadIndices;

	quad.program = hyp::CreateRef<hyp::ShaderProgram>("assets/shaders/quad.vert",
	    "assets/shaders/quad.frag");

	quad.program->link();
	quad.program->setBlockBinding("Camera", 0);
	quad.program->setBlockBinding("Transform", 1);
	quad.program->setBlockBinding("Lights", 2);

	// initialize the texture sampler slots in shader
	quad.program->use();
	for (int i = 0; i < MaxTextureSlots; i++)
	{
		std::string name = "textures[" + std::to_string(i) + "]";
		quad.program->setInt(name, i);
	}

	quad.transformBuffer = hyp::UniformBuffer::create(sizeof(glm::mat4) * MaxQuad, 1);

	quad.vertexPos[0] = { +0.5f, +0.5f, 0.0, 1.f };
	quad.vertexPos[1] = { -0.5f, +0.5f, 0.0, 1.f };
	quad.vertexPos[2] = { -0.5f, -0.5f, 0.0, 1.f };
	quad.vertexPos[3] = { +0.5f, -0.5f, 0.0, 1.f };

	quad.uvCoords[0] = { 1.f, 1.f };
	quad.uvCoords[1] = { 0.f, 1.f };
	quad.uvCoords[2] = { 0.f, 0.f };
	quad.uvCoords[3] = { 1.f, 0.f };
}

void utils::flushQuad() {
	auto& quad = s_renderer.quad;
	uint32_t size = quad.vertices.size();
	if (size == 0)
	{
		return;
	}

	quad.vbo->setData(quad.vertices.data(), size * sizeof(QuadVertex));

	auto& lighting = s_renderer.lighting;
	quad.transformBuffer->setData(quad.transforms.data(), quad.transforms.size() * sizeof(glm::mat4));

	quad.program->use();
	quad.program->setBool("enableLighting", lighting.enabled);
	if (lighting.enabled)
	{
		quad.program->setInt("noLights", lighting.lightCount);
		lighting.uniformBuffer->setData(lighting.lights.data(), lighting.lights.size() * sizeof(Light));
	}
	else
	{
		quad.program->setInt("noLights", 0);
	}

	for (uint32_t i = 0; i < quad.textureSlotIndex; i++)
	{
		quad.textureSlots[i]->bind(i);
	}
	hyp::RenderCommand::drawIndexed(quad.vao, quad.indexCount);

	s_renderer.stats.quadCount += quad.transforms.size(); // each transform count presents a quad (4 vertices)
	s_renderer.stats.drawCalls++;
}

void utils::nextQuadBatch() {
	utils::flushQuad();
	s_renderer.quad.reset();
}

/*Line Data*/
void utils::initLine() {
	auto& line = s_renderer.line;

	line.vao = hyp::VertexArray::create();
	line.vbo = hyp::VertexBuffer::create(MaxVertices * sizeof(LineVertex));

	line.vbo->setLayout({
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec3, "aPos", false),
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec4, "aColor", false),
	});

	line.vao->addVertexBuffer(s_renderer.line.vbo);
	line.vertices.clear();
	line.vertices.resize(MaxVertices);

	line.program = hyp::CreateRef<hyp::ShaderProgram>(
	    "assets/shaders/line.vert", "assets/shaders/line.frag");
	line.program->link();
	line.program->setBlockBinding("Camera", 0);
}

void utils::flushLine() {
	auto& line = s_renderer.line;
	size_t size = line.vertices.size();
	if (size == 0)
	{
		return;
	}

	line.vao->bind();
	line.vbo->setData(&line.vertices[0], size * sizeof(LineVertex));
	line.program->use();

	hyp::RenderCommand::drawLines(line.vao, size);

	s_renderer.stats.lineCount += size * 0.5;
	s_renderer.stats.drawCalls++;
}

void utils::nextLineBatch() {
	utils::flushLine();
	s_renderer.line.reset();
}

/* Circle Data */
void utils::initCircle() {
	auto& circle = s_renderer.circle;

	circle.vertices.clear();
	circle.vertices.resize(MaxVertices);

	circle.vao = hyp::VertexArray::create();

	circle.vbo = hyp::VertexBuffer::create(MaxVertices * sizeof(CircleVertex));
	auto& layout = hyp::BufferLayout({
	    { hyp::ShaderDataType::Vec3, "aWorldPosition" },
	    { hyp::ShaderDataType::Vec3, "aLocalPosition" },
	    { hyp::ShaderDataType::Vec4, "aColor" },
	    { hyp::ShaderDataType::Float, "aThickness" },
	    { hyp::ShaderDataType::Float, "aFade" },
	});
	circle.vbo->setLayout(layout);

	uint32_t* indices = new uint32_t[MaxIndices];

	int offset = 0;
	int i = 0;
	for (i = 0; i < MaxIndices; i += 6)
	{
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}

	auto& circleIndexBuffer = hyp::CreateRef<hyp::ElementBuffer>(indices, MaxIndices);
	circle.vao->addVertexBuffer(circle.vbo);
	circle.vao->setIndexBuffer(circleIndexBuffer);

	delete[] indices;

	circle.program = hyp::ShaderProgram::create("assets/shaders/circle.vert",
	    "assets/shaders/circle.frag");

	circle.program->link();
	circle.program->setBlockBinding("Camera", 0);
}

void utils::flushCircle() {
	auto& circle = s_renderer.circle;
	size_t size = circle.vertices.size();

	if (!circle.indexCount)
	{
		return;
	}

	circle.vbo->setData(circle.vertices.data(), (uint32_t)size * sizeof(CircleVertex));

	circle.program->use();
	hyp::RenderCommand::drawIndexed(circle.vao, circle.indexCount);
}

void utils::nextCircleBatch() {
	flushCircle();
	s_renderer.circle.reset();
}

/* Text Data */

void utils::initText() {
	auto& text = s_renderer.text;

	text.vao = hyp::VertexArray::create();
	text.vbo = hyp::VertexBuffer::create(MaxVertices * sizeof(TextVertex));
	text.vbo->setLayout({
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec3, "aPos", false),
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec4, "aColor", false),
	    hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec2, "aUV", false),
	});

	text.vao->addVertexBuffer(text.vbo);
	text.vertices.clear();
	text.vertices.resize(MaxVertices);

	uint32_t* indices = new uint32_t[MaxIndices];
	int offset = 0;
	int i = 0;

	for (i = 0; i < MaxIndices; i += 6)
	{
		indices[i + 0] = offset + 0;
		indices[i + 1] = offset + 1;
		indices[i + 2] = offset + 2;

		indices[i + 3] = offset + 2;
		indices[i + 4] = offset + 3;
		indices[i + 5] = offset + 0;

		offset += 4;
	}

	auto& textIndexBuffer = hyp::CreateRef<hyp::ElementBuffer>(indices, MaxIndices);

	text.vao->setIndexBuffer(textIndexBuffer);
	delete[] indices;

	text.program = hyp::ShaderProgram::create("assets/shaders/text.vert",
	    "assets/shaders/text.frag");

	text.program->link();
	text.program->setBlockBinding("Camera", 0);
}

void utils::flushText() {
	auto& text = s_renderer.text;
	size_t size = text.vertices.size();

	if (!text.indexCount)
	{
		return;
	}

	text.vbo->setData(text.vertices.data(), (uint32_t)size * sizeof(TextVertex));

	text.program->use();
	text.fontAtlasTexture->bind(0);
	hyp::RenderCommand::drawIndexed(text.vao, text.indexCount);
}

void utils::nextTextBatch() {
	flushText();
	s_renderer.text.reset();
}

hyp::Renderer2D::Stats hyp::Renderer2D::getStats() {
	return s_renderer.stats;
}
