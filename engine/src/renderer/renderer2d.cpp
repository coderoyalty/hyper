#include <renderer/renderer2d.hpp>
#include <renderer/vertex_array.hpp>
#include <renderer/vertex_buffer.hpp>
#include <renderer/shader.hpp>

const uint32_t maxQuad = 500;
const uint32_t maxVertices = maxQuad * 4;

namespace hyp {



	struct Vertex
	{
		glm::vec3 pos = glm::vec3(0.0);
		glm::vec4 color = glm::vec4(1.0);
		glm::vec2 uv;
	};

	struct RendererData {
		hyp::Ref<hyp::VertexBuffer> vbo;
		hyp::Ref<hyp::VertexArray> vao;
		hyp::Ref<hyp::ShaderProgram> program;

		/*Vertex* vbase = nullptr;
		Vertex* vptr = nullptr;*/
		std::vector<Vertex> data;

		glm::vec4 vertexPos[6];

	};


	static RendererData renderer;

	void hyp::Renderer2D::init() {
		renderer.vao = hyp::CreateRef<hyp::VertexArray>();
		renderer.vbo = hyp::CreateRef<hyp::VertexBuffer>(maxVertices * sizeof(Vertex));

		renderer.vbo->setLayout({
				hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec3, "aPos", false),
				hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec4, "aColor", false),
				hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec2, "aUV", false)
			});

		renderer.vao->addVertexBuffer(renderer.vbo);
		renderer.data.clear();
		renderer.data.resize(maxVertices);

		renderer.program = hyp::CreateRef<hyp::ShaderProgram>("assets/shaders/quad.vert",
			"assets/shaders/quad.frag");

		renderer.program->link();

		renderer.vertexPos[0] = { +1.0, +1.0, 0.0, 1.f };
		renderer.vertexPos[1] = { 0.0f, +1.0, 0.0, 1.f };
		renderer.vertexPos[2] = { 0.0f, 0.0f, 0.0, 1.f };
		renderer.vertexPos[3] = { 0.0f, 0.0f, 0.0, 1.f };
		renderer.vertexPos[4] = { +1.0, 0.0f, 0.0, 1.f };
		renderer.vertexPos[5] = { +1.0, +1.0, 0.0, 1.f };
	}

	void Renderer2D::deinit()
	{
		renderer.data.clear();
	};


	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(size, 0.f));

		for (int i = 0; i < 6; i++) {
			Vertex vertex;
			vertex.pos = model * renderer.vertexPos[i];
			vertex.color = color;
			vertex.uv = { 0.f, 0.f };

			renderer.data.push_back(vertex);
		}
	}

	void Renderer2D::startBatch()
	{
		renderer.data.clear();
	}


	void Renderer2D::flush() {
		size_t size = renderer.data.size();
		if (renderer.data.size() == 0) {
			return;
		}
		renderer.vao->bind();
		renderer.vbo->setData(renderer.data.data(), size * sizeof(Vertex));
		renderer.program->use();
		glDrawArrays(GL_TRIANGLES, 0, size);
	}

	void Renderer2D::nextBatch()
	{
		flush();
		startBatch();
	}

	void Renderer2D::beginScene(const glm::mat4& viewProjectionMatrix)
	{
		startBatch();

		renderer.program->use();
		renderer.program->setMat4("viewProj", viewProjectionMatrix);
	}

	void Renderer2D::endScene() {
		flush();
	}

}
