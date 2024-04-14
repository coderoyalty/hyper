#pragma once
#ifndef HYP_RENDER_COMMAND
	#define HYP_RENDER_COMMAND

	#include <glm/glm.hpp>
	#include <renderer/vertex_array.hpp>

namespace hyp {
	class RenderCommand {
	public:
		static void init();

		static void setClearColor(const glm::vec4& color);
		static void setClearColor(float r, float g, float b, float a);
		static void clear();

		static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);

		static void drawIndexed(const hyp::Ref<hyp::VertexArray>& vao, uint32_t indexCount = 0);
		static void drawLines(const hyp::Ref<hyp::VertexArray>& vao, uint32_t vertexCount);
		static void setLineWidth(float width);
	};
}

#endif