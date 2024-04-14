#pragma once

#include <core/application.hpp>
#include <renderer/vertex_array.hpp>
#include <renderer/orthographic_controller.hpp>
#include <renderer/texture.hpp>
#include <renderer/shader.hpp>
#include <renderer/renderer2d.hpp>
#include <renderer/framebuffer.hpp>
#include <imgui.h>

class TexturedQuadLayer : public hyp::Layer {
public:
	TexturedQuadLayer()
	    : controller(600.f, 600.f) {
		hyp::FramebufferSpecification fbSpec;
		fbSpec.attachment = hyp::FbAttachmentSpecification({ { hyp::FbTextureFormat::RGBA } });
		fbSpec.width = 600;
		fbSpec.height = 600;
		framebuffer = hyp::Framebuffer::create(fbSpec);

		float vertices[6 * 5] = {
			-0.0f,
			-0.0f,
			0.f,
			0.f,
			0.f,
			-0.0f,
			+1.0f,
			0.f,
			0.f,
			1.f,
			+1.0f,
			+1.0f,
			0.f,
			1.f,
			1.f,
			+1.0f,
			+1.0f,
			0.f,
			1.f,
			1.f,
			+1.0f,
			-0.0f,
			0.f,
			1.f,
			0.f,
			-0.0f,
			-0.0f,
			0.f,
			0.f,
			0.f,
		};
		vao = hyp::VertexArray::create();
		auto& vbo = hyp::VertexBuffer::create(vertices, sizeof(vertices));

		vbo->setLayout({ { hyp::ShaderDataType::Vec3, "aPos" }, { hyp::ShaderDataType::Vec2, "aTex" } });

		vao->bind();
		vao->addVertexBuffer(vbo);

		wallTexture = hyp::CreateRef<hyp::Texture>("assets/wall.jpg");

		program = hyp::ShaderProgram::create("assets/vertex.vert", "assets/fragment.frag");
		program->link();

		glm::mat4 model(1.f);
		float size = 150.f;
		model = glm::translate(model, glm::vec3(600.f - size, 600.f - size, 0.f));
		model = glm::scale(model, glm::vec3(size, size, 0.f));

		glm::mat4 viewProj = glm::ortho(0.f, 600.f, 600.f, 0.f, -1.f, +1.f);

		program->use();
		program->setMat4("uViewProj", viewProj);
		program->setMat4("model", model);
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

		framebuffer->bind();
		hyp::RenderCommand::setClearColor(0.3f, 0.3f, 0.3f, 1.f);
		hyp::RenderCommand::clear();

		hyp::Renderer2D::beginScene(controller.getCamera().getViewProjectionMatrix());
		hyp::Renderer2D::drawQuad({ 0.0, 0.f, 0.f }, { 200.f, 200.f }, wallTexture, 1.f, glm::vec4(1.f));
		hyp::Renderer2D::endScene();
		framebuffer->unbind();
	}

	virtual void onUIRender() override {
		ImGui::Begin("Offscreen rendering");
		auto textureId = framebuffer->getColorAttachmentId();
		auto spec = framebuffer->getSpecification();
		ImGui::Image((void*)(uintptr_t)textureId, ImVec2(spec.width, spec.height),
		    ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
		ImGui::End();
	}

	hyp::Ref<hyp::Texture> wallTexture;
	hyp::Ref<hyp::ShaderProgram> program;
	hyp::Ref<hyp::VertexArray> vao;
	hyp::OrthoGraphicCameraController controller;

	hyp::Ref<hyp::Framebuffer> framebuffer;
};
