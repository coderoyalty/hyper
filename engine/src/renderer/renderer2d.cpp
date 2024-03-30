#include <renderer/renderer2d.hpp>
#include <renderer/vertex_array.hpp>
#include <renderer/vertex_buffer.hpp>
#include <renderer/shader.hpp>
#include "uniform_buffer.hpp"

const uint32_t maxQuad = 500;
const uint32_t maxVertices = maxQuad * 4;

namespace hyp {
	namespace utils {
		static void init_quad();
		static void init_line();

		static void flush_quad();
		static void flush_line();
	}

	struct QuadVertex
	{
		glm::vec3 pos = glm::vec3(0.0);
		glm::vec4 color = glm::vec4(1.0);
		glm::vec2 uv{};
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

	struct Quad : public RenderEntity {
		std::vector<QuadVertex> vertices;
	};

	struct Line : public RenderEntity {
		std::vector<LineVertex> vertices;
		float lineWidth = 2.f;
	};


	struct RendererData {

		// entity
		Quad quad;
		Line line;

		glm::vec4 vertexPos[6] = {};

		struct CameraData {
			glm::mat4 viewProjection;
		};

		CameraData cameraBuffer{};
		hyp::Shared<hyp::UniformBuffer> cameraUniformBuffer;

	};


	static RendererData renderer;

	void hyp::Renderer2D::init() {
		utils::init_quad();
		utils::init_line();

		HYP_INFO("Initialize 2D Renderer");

		renderer.cameraUniformBuffer = hyp::CreateRef<hyp::UniformBuffer>(sizeof(RendererData::CameraData), 0);
		glEnable(GL_LINE_SMOOTH);
	}

	void Renderer2D::deinit()
	{
		renderer.quad.vertices.clear();
		renderer.line.vertices.clear();
		HYP_INFO("Destroyed 2D Renderer");

	};


	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(size, 0.f));

		for (int i = 0; i < 6; i++) {
			QuadVertex vertex;
			vertex.pos = model * renderer.vertexPos[i];
			vertex.color = color;
			vertex.uv = { 0.f, 0.f };

			renderer.quad.vertices.push_back(vertex);
		}
	}

	void Renderer2D::drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color) {
		LineVertex v0, v1;

		v0.position = p1;
		v1.position = p2;

		v0.color = v1.color = color;

		renderer.line.vertices.push_back(v0);
		renderer.line.vertices.push_back(v1);
	}

	void Renderer2D::startBatch()
	{
		renderer.quad.vertices.clear();
		renderer.line.vertices.clear();
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

		renderer.cameraBuffer.viewProjection = viewProjectionMatrix;
		renderer.cameraUniformBuffer->setData(&renderer.cameraBuffer, sizeof(RendererData::CameraData));
	}

	void Renderer2D::endScene() {
		flush();
	}
}

void hyp::utils::init_quad() {
	auto& quad = renderer.quad;

	quad.vao = hyp::CreateRef<hyp::VertexArray>();
	quad.vbo = hyp::CreateRef<hyp::VertexBuffer>(maxVertices * sizeof(QuadVertex));
	quad.vbo->setLayout({
			hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec3, "aPos", false),
			hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec4, "aColor", false),
			hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec2, "aUV", false)
		});

	quad.vao->addVertexBuffer(renderer.quad.vbo);
	quad.vertices.clear();
	quad.vertices.resize(maxVertices);

	quad.program = hyp::CreateRef<hyp::ShaderProgram>("assets/shaders/quad.vert",
		"assets/shaders/quad.frag");
	quad.program->setBlockBinding("Camera", 0);


	quad.program->link();

	renderer.vertexPos[0] = { +0.5f, +0.5f, 0.0, 1.f };
	renderer.vertexPos[1] = { -0.5f, +0.5f, 0.0, 1.f };
	renderer.vertexPos[2] = { -0.5f, -0.5f, 0.0, 1.f };
	renderer.vertexPos[3] = { -0.5f, -0.5f, 0.0, 1.f };
	renderer.vertexPos[4] = { +0.5f, -0.5f, 0.0, 1.f };
	renderer.vertexPos[5] = { +0.5f, +0.5f, 0.0, 1.f };
}

void hyp::utils::init_line() {
	auto& line = renderer.line;


	line.vao = hyp::CreateRef<hyp::VertexArray>();
	line.vbo = hyp::CreateRef<hyp::VertexBuffer>(maxVertices * sizeof(LineVertex));

	line.vbo->setLayout({
		hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec3, "aPos", false),
		hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec4, "aColor", false),
		});

	line.vao->addVertexBuffer(renderer.line.vbo);
	line.vertices.clear();
	line.vertices.resize(maxVertices);

	line.program = hyp::CreateRef<hyp::ShaderProgram>(
		"assets/shaders/line.vert", "assets/shaders/line.frag");
	line.program->setBlockBinding("Camera", 0);


	line.program->link();
}

void hyp::utils::flush_quad()
{
	auto& quad = renderer.quad;
	size_t size = quad.vertices.size();
	if (size == 0) {
		return;
	}

	quad.vao->bind();
	quad.vbo->setData(quad.vertices.data(), size * sizeof(QuadVertex));
	quad.program->use();
	glDrawArrays(GL_TRIANGLES, 0, size);
}

void hyp::utils::flush_line() {
	auto& line = renderer.line;
	size_t size = line.vertices.size();
	if (size == 0) {
		return;
	}

	line.vao->bind();
	line.vbo->setData(&line.vertices[0], size * sizeof(LineVertex));
	line.program->use();

	glLineWidth(line.lineWidth);
	glDrawArrays(GL_LINES, 0, size);
}
