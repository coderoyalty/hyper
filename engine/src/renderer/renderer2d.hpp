#pragma once
#ifndef HYPER_RENDERER_2D_HPP
	#define HYPER_RENDERER_2D_HPP

	#include <glm/glm.hpp>
	#include <glm/gtc/matrix_transform.hpp>
	#include <renderer/render_command.hpp>
	#include <renderer/texture.hpp>
	#include <renderer/font.hpp>

namespace hyp {
	struct Light
	{
		glm::vec4 position;
		glm::vec4 color;
	};

	class Renderer2D {
	public:
		struct Stats
		{
			int drawCalls = 0;
			int quadCount = 0;
			int lineCount = 0;

			int getQuadCount() const { return quadCount; }
			int getLineCount() const { return lineCount; }
		};

	public:
		static void init();
		static void deinit();

		static void enableLighting(bool value);

	public:
		static void beginScene(const glm::mat4& viewProjectionMatrix);
		static void endScene();

		static void addLight(const Light& light);

	public:
		static void drawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void drawQuad(const glm::mat4& transform, hyp::Ref<hyp::Texture2D>& texture, float tilingFactor = 1.f, const glm::vec4& color = glm::vec4(1.f));

		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size,
		    hyp::Ref<hyp::Texture2D> texture, float tilingFactor = 1.f, const glm::vec4& color = glm::vec4(1.0));

	public:
		static void drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color = glm::vec4(1.0));
		static void drawCircle(const glm::mat4& transform, float thickness, float fade, const glm::vec4& color = glm::vec4(1.f));

	public:
		struct TextParams
		{
			glm::vec4 color { 1.f };
			// range 0.0 - 1.0
			float leading = 0.f; // line-spacing
			float fontSize = 48.f;
		};
		static void drawString(const std::string& text, hyp::Ref<hyp::Font> font, const glm::mat4& transform, const TextParams& textParams);

	public:
		static Stats getStats();

	private:
		static void startBatch();
		static void nextBatch();
		static void flush();

	private:
	};
} // namespace  hyp

	#ifndef RENDERER_2D_DATA_STRUCTURES

namespace hyp {
	struct QuadVertex;
	struct CircleVertex;
	struct LineVertex;
	struct TextVertex;
	struct RenderEntity;
}

	#else

		#include <renderer/vertex_array.hpp>
		#include <renderer/vertex_buffer.hpp>
		#include <renderer/shader.hpp>
		#include "uniform_buffer.hpp"
		#include <renderer/element_buffer.hpp>
		#include <renderer/render_command.hpp>
		#include <array>

/* Constants */

const uint32_t MaxLight = 32;
const uint32_t MaxTextureSlots = 32;
const uint32_t MaxQuad = 1000;
const uint32_t MaxCircles = MaxQuad;
const uint32_t MaxVertices = MaxQuad * 4;
const uint32_t MaxIndices = MaxQuad * 6;

namespace utils {
	static void initQuad();
	static void flushQuad();
	static void nextQuadBatch();

	static void initLine();
	static void flushLine();
	static void nextLineBatch();

	static void initCircle();
	static void flushCircle();
	static void nextCircleBatch();

	static void initText();
	static void flushText();
	static void nextTextBatch();
}

namespace hyp {
	struct QuadVertex
	{
		glm::vec3 pos = glm::vec3(0.0);
		glm::vec4 color = glm::vec4(1.0);
		glm::vec2 uv {};
		int transformIndex = 0;
		float textureIndex = 0;
		float tilingFactor = 1.f; // no. of times a texture is repeated.
	};

	struct LineVertex
	{
		glm::vec3 position = { 0.f, 0.f, 0.f };
		glm::vec4 color = glm::vec4(1.f);
	};

	struct CircleVertex
	{
		glm::vec3 worldPosition;
		glm::vec3 localPosition;
		glm::vec4 color;
		float thickness;
		float fade;
	};

	struct TextVertex
	{
		glm::vec3 position = { 0.f, 0.f, 0.f };
		glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };
		glm::vec2 uvCoord = {0.f, 0.f};
	};

	struct RenderEntity
	{
		hyp::Ref<hyp::VertexBuffer> vbo;
		hyp::Ref<hyp::VertexArray> vao;
		hyp::Ref<hyp::ShaderProgram> program;
	};

	struct QuadData : public RenderEntity
	{
		// quad vertices
		std::vector<QuadVertex> vertices;
		uint32_t indexCount = 0;

		// transformation info
		std::vector<glm::mat4> transforms;
		hyp::Shared<hyp::UniformBuffer> transformBuffer;
		int transformIndexCount = 0;

		// textures
		std::array<hyp::Ref<hyp::Texture2D>, MaxTextureSlots> textureSlots;
		hyp::Ref<hyp::Texture2D> defaultTexture;
		uint32_t textureSlotIndex = 1; // 1 instead of 0 because there's already an existing texture -- defaultTexture --

		glm::vec4 vertexPos[4] = {};
		glm::vec2 uvCoords[4] = {};

		void reset() {
			vertices.clear();
			transforms.clear();
			indexCount = 0;
			transformIndexCount = 0;
			textureSlotIndex = 1;
		}
	};

	struct LineData : public RenderEntity
	{
		std::vector<LineVertex> vertices;
		virtual void reset() {
			vertices.clear();
		}
	};

	struct CircleData : public RenderEntity
	{
		std::vector<CircleVertex> vertices;
		uint32_t indexCount = 0;

		virtual void reset() {
			vertices.clear();
			indexCount = 0;
		}
	};

	struct TextData : public RenderEntity
	{
		std::vector<TextVertex> vertices;
		uint32_t indexCount = 0;

		hyp::Ref<hyp::Texture2D> fontAtlasTexture;

		virtual void reset() {
			vertices.clear();
			indexCount = 0;
		}
	};

	//TODO: remove the experimental lighting support, (looking to use deferred rendering to support this)

	///TODO: support for other lighting settings such as:
	/// 1. the constant, linear and quadratic value when calculation attenuation
	/// 2. providing ambient, diffuse and specular color for each light
	struct LightingData
	{
		std::vector<hyp::Light> lights;
		hyp::Shared<hyp::UniformBuffer> uniformBuffer;
		int lightCount = 0;
		bool enabled = false;
	};

	struct RendererData
	{
		// entity data
		QuadData quad;
		LineData line;
		CircleData circle;
		TextData text;
		LightingData lighting;

		struct CameraData
		{
			glm::mat4 viewProjection;
		};

		CameraData cameraBuffer {};
		hyp::Shared<hyp::UniformBuffer> cameraUniformBuffer;

		Renderer2D::Stats stats;
	};
}

	#endif

#endif