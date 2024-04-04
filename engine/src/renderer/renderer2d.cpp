#include <renderer/renderer2d.hpp>
#include <renderer/vertex_array.hpp>
#include <renderer/vertex_buffer.hpp>
#include <renderer/shader.hpp>
#include "uniform_buffer.hpp"
#include "renderer/element_buffer.hpp"
#include "renderer/render_command.hpp"


const uint32_t maxLight = 32;

namespace hyp {
	namespace utils {
		static void init_quad();
		static void flush_quad();

		static void init_line();
		static void flush_line();
	}

	struct QuadVertex
	{
		glm::vec3 pos = glm::vec3(0.0);
		glm::vec4 color = glm::vec4(1.0);
		glm::vec2 uv{};
		int index = 0;
	};

	struct LineVertex {
		glm::vec3 position = { 0.f, 0.f, 0.f };
		glm::vec4 color = glm::vec4(1.f);
	};


	struct RenderEntity {
		hyp::Ref<hyp::VertexBuffer> vbo;
		hyp::Ref<hyp::VertexArray> vao;
		hyp::Ref<hyp::ShaderProgram> program;
	};

	struct QuadData : public RenderEntity {
		std::vector<QuadVertex> vertices;
		std::vector<glm::mat4> transforms;
		hyp::Shared<hyp::UniformBuffer> uniformBuffer;

		glm::vec4 vertexPos[4] = {};
		glm::vec2 uvCoords[6] = {};
		int entityIndexCount = 0;

		uint32_t indexCount = 0;

		void reset() {
			vertices.clear();
			indexCount = 0;
			entityIndexCount = 0;
		}
	};

	struct LineData : public RenderEntity {
		std::vector<LineVertex> vertices;
		
		void reset() {
			vertices.clear();
		}
	};

	///TODO: support for other lighting settings such as:
	/// 1. the constant, linear and quadratic value when calculation attenuation
	/// 2. providing ambient, diffuse and specular color for each light
	struct LightingData {
		std::vector<hyp::Light> lights;
		hyp::Shared<hyp::UniformBuffer> uniformBuffer;
		int lightCount = 0;
		bool enabled = false;
	};

	struct RendererData {
		//

		static const uint32_t maxQuad = 1000;
		static const uint32_t maxVertices = maxQuad * 4;
		static const uint32_t maxIndices = maxQuad * 6;
		
		// entity
		QuadData quad;
		LineData line;
		LightingData lighting;

		struct CameraData {
			glm::mat4 viewProjection;
		};


		CameraData cameraBuffer{};
		hyp::Shared<hyp::UniformBuffer> cameraUniformBuffer;

		Renderer2D::Stats stats;
	};

	static RendererData s_renderer;

	void hyp::Renderer2D::init() {
		utils::init_quad();
		utils::init_line();

		HYP_INFO("Initialize 2D Renderer");

		s_renderer.cameraUniformBuffer = hyp::UniformBuffer::create(sizeof(RendererData::CameraData), 0);
		s_renderer.lighting.uniformBuffer = hyp::UniformBuffer::create(sizeof(Light) * maxLight, 2);	
	}

	void Renderer2D::deinit()
	{
		s_renderer.quad.vertices.clear();
		s_renderer.line.vertices.clear();
		HYP_INFO("Destroyed 2D Renderer");

	}
	void Renderer2D::enableLighting(bool value)
	{
		s_renderer.lighting.enabled = value;
	};

	void Renderer2D::addLight(const Light& light)
	{
		auto& lighting = s_renderer.lighting;

		if (lighting.lightCount >= maxLight) return;

		lighting.lights.push_back(light);
		lighting.lightCount++;
	}

	void Renderer2D::startBatch()
	{
		s_renderer.quad.reset();
		s_renderer.line.reset();

		s_renderer.stats.drawCalls = 0;
		s_renderer.stats.lineCount = 0;
		s_renderer.stats.quadCount = 0;

		// lightings
		s_renderer.lighting.lights.clear();
		s_renderer.lighting.lightCount = 0;
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		auto& quad = s_renderer.quad;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position + glm::vec3(size / 2.f, 0.f));
		model = glm::scale(model, glm::vec3(size, 0.f));

		int quadVertexCount = 4;

		for (int i = 0; i < quadVertexCount; i++) {
			QuadVertex vertex;
			vertex.pos = quad.vertexPos[i];
			vertex.color = color;
			vertex.uv = quad.uvCoords[i];
			vertex.index = quad.entityIndexCount;

			quad.vertices.push_back(vertex);
		}

		quad.transforms.push_back(model);
		quad.entityIndexCount++;

		if (quad.transforms.size() == RendererData::maxQuad) {
			nextBatch();
		}

		s_renderer.quad.indexCount += 6;
	}

	void Renderer2D::drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color) {
		LineVertex v0, v1;

		v0.position = p1;
		v1.position = p2;

		v0.color = v1.color = color;

		s_renderer.line.vertices.push_back(v0);
		s_renderer.line.vertices.push_back(v1);
	}


	void Renderer2D::flush() {
		utils::flush_quad();
		utils::flush_line();
	}

	void Renderer2D::nextBatch()
	{
		flush();
		startBatch();
	}

	void Renderer2D::beginScene(const glm::mat4& viewProjectionMatrix)
	{
		startBatch();

		s_renderer.cameraBuffer.viewProjection = viewProjectionMatrix;
		s_renderer.cameraUniformBuffer->setData(&s_renderer.cameraBuffer, sizeof(RendererData::CameraData));
	}

	void Renderer2D::endScene() {
		flush();
	}
}

/*Quad Data*/

void hyp::utils::init_quad() {
	auto& quad = s_renderer.quad;

	quad.vao = hyp::VertexArray::create();
	quad.vbo = hyp::VertexBuffer::create(RendererData::maxVertices * sizeof(QuadVertex));

	quad.vbo->setLayout({
			hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec3, "aPos", false),
			hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec4, "aColor", false),
			hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec2, "aUV", false),
			hyp::VertexAttribDescriptor(hyp::ShaderDataType::Int, "aIndex", false)
		});

	quad.vao->addVertexBuffer(s_renderer.quad.vbo);
	quad.vertices.clear();
	quad.vertices.resize(RendererData::maxVertices);

	uint32_t* quadIndices = new uint32_t[RendererData::maxIndices];

	int offset = 0;
	int i = 0;
	for (i = 0; i < RendererData::maxIndices; i += 6) {
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}
	hyp::Ref<hyp::ElementBuffer> elementBuffer = hyp::CreateRef<hyp::ElementBuffer>(quadIndices, RendererData::maxIndices);
	quad.vao->setIndexBuffer(elementBuffer);

	delete[] quadIndices;

	quad.program = hyp::CreateRef<hyp::ShaderProgram>("assets/shaders/quad.vert",
		"assets/shaders/quad.frag");
	quad.program->setBlockBinding("Camera", 0);

	quad.program->link();
	quad.program->setBlockBinding("Camera", 0);
	quad.program->setBlockBinding("Transform", 1);
	quad.program->setBlockBinding("Lights", 2);


	quad.uniformBuffer = hyp::UniformBuffer::create(sizeof(glm::mat4) * RendererData::maxQuad, 1);

	quad.vertexPos[0] = { +0.5f, +0.5f, 0.0, 1.f };
	quad.vertexPos[1] = { -0.5f, +0.5f, 0.0, 1.f };
	quad.vertexPos[2] = { -0.5f, -0.5f, 0.0, 1.f };
	quad.vertexPos[3] = { +0.5f, -0.5f, 0.0, 1.f };
}

void hyp::utils::flush_quad()
{
	auto& quad = s_renderer.quad;
	size_t size = quad.vertices.size();
	if (size == 0) {
		return;
	}

	quad.vao->bind();
	quad.vbo->setData(quad.vertices.data(), size * sizeof(QuadVertex));

	auto& lighting = s_renderer.lighting;

	quad.program->use();
	quad.uniformBuffer->setData(quad.transforms.data(), quad.transforms.size() * sizeof(glm::mat4));

	quad.program->setBool("enableLighting", lighting.enabled);
	if (lighting.enabled) {
		quad.program->setInt("noLights", lighting.lightCount);
		lighting.uniformBuffer->setData(lighting.lights.data(), lighting.lights.size() * sizeof(Light));
	}
	else {
		quad.program->setInt("noLights", 0);
	}

	hyp::RenderCommand::drawIndexed(quad.vao, quad.indexCount);

	s_renderer.stats.quadCount += quad.entityIndexCount;
	s_renderer.stats.drawCalls++;
}

/*Line Data*/
void hyp::utils::init_line() {
	auto& line = s_renderer.line;


	line.vao = hyp::VertexArray::create();
	line.vbo = hyp::VertexBuffer::create(RendererData::maxVertices * sizeof(LineVertex));

	line.vbo->setLayout({
		hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec3, "aPos", false),
		hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec4, "aColor", false),
		});

	line.vao->addVertexBuffer(s_renderer.line.vbo);
	line.vertices.clear();
	line.vertices.resize(RendererData::maxVertices);

	line.program = hyp::CreateRef<hyp::ShaderProgram>(
		"assets/shaders/line.vert", "assets/shaders/line.frag");
	line.program->setBlockBinding("Camera", 0);

	line.program->link();
}

void hyp::utils::flush_line() {
	auto& line = s_renderer.line;
	size_t size = line.vertices.size();
	if (size == 0) {
		return;
	}

	line.vao->bind();
	line.vbo->setData(&line.vertices[0], size * sizeof(LineVertex));
	line.program->use();

	hyp::RenderCommand::drawLines(line.vao, size);

	s_renderer.stats.lineCount += size / 2;
	s_renderer.stats.drawCalls++;
}

hyp::Renderer2D::Stats hyp::Renderer2D::getStats() {
	return s_renderer.stats;
}
