#pragma once
#ifndef HYPER_RENDERER_2D_HPP
#define HYPER_RENDERER_2D_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <renderer/render_command.hpp>

namespace hyp
{
	struct Light {
		glm::vec4 position;
		glm::vec4 color;
	};


	class Renderer2D
	{
	public:
		struct Stats {
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
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void drawLine(const glm::vec3& p1, const glm::vec3& p2, const glm::vec4& color = glm::vec4(1.0));
	public:
		static Stats getStats();
	private:
		static void startBatch();
		static void nextBatch();
		static void flush();
	private:
	};
} // namespace  hyp

#endif