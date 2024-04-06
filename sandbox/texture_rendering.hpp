#pragma once

#include <core/application.hpp>
#include <renderer/vertex_array.hpp>
#include <renderer/orthographic_controller.hpp>
#include <renderer/texture.hpp>
#include <renderer/shader.hpp>
#include <renderer/renderer2d.hpp>

class TexturedQuadLayer : public hyp::Layer {
public:
	TexturedQuadLayer()
		: controller(600.f, 600.f)
	{

		float vertices[6 * 5] = {
			-0.5f, -0.5f, 0.f, 0.f, 0.f,
			-0.5f, +0.5f, 0.f, 0.f, 1.f,
			+0.5f, +0.5f, 0.f, 1.f, 1.f,
			+0.5f, +0.5f, 0.f, 1.f, 1.f,
			+0.5f, -0.5f, 0.f, 1.f, 0.f,
			-0.5f, -0.5f, 0.f, 0.f, 0.f,
		};
		vao = hyp::VertexArray::create();
		auto& vbo = hyp::VertexBuffer::create(vertices, sizeof(vertices));

		vbo->setLayout({ {hyp::ShaderDataType::Vec3, "aPos"}, {hyp::ShaderDataType::Vec2, "aTex"} });

		vao->bind();
		vao->addVertexBuffer(vbo);

		wallTexture = hyp::CreateRef<hyp::Texture>("assets/wall.jpg");
		redTexture = hyp::CreateRef<hyp::Texture>(hyp::TextureSpecification());

		uint32_t redColor = 0xff0000ff;
		redTexture->setData(&redColor, sizeof(uint32_t));

		program = hyp::ShaderProgram::create("assets/vertex.vert", "assets/fragment.frag");
		program->link();
		program->use();
	}

	virtual void onAttach() override {
	}

	virtual void onDetach() override {

	}

	virtual void onEvent(hyp::Event& event) override {
		controller.onEvent(event);
	}

	virtual void onUpdate(float dt) override {
		controller.onUpdate(dt);
		hyp::RenderCommand::setClearColor(0.3f, 0.3f, 0.3f, 1.f);
		hyp::RenderCommand::clear();

		hyp::Renderer2D::beginScene(controller.getCamera().getViewProjectionMatrix());
		hyp::Renderer2D::drawQuad({ 0.0, 0.f, 0.f }, { 20.f, 20.f }, wallTexture, glm::vec4(1.f));
		hyp::Renderer2D::drawQuad({ 20.0, 20.f, 0.f }, { 20.f, 20.f }, redTexture, glm::vec4(1.f));
		hyp::Renderer2D::endScene();
	}

	hyp::Ref<hyp::Texture> wallTexture;
	hyp::Ref<hyp::Texture> redTexture;
	hyp::Ref<hyp::ShaderProgram> program;
	hyp::Ref<hyp::VertexArray> vao;
	hyp::OrthoGraphicCameraController controller;
};
