#include "render_command.hpp"

void hyp::RenderCommand::init() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
}

void hyp::RenderCommand::setClearColor(const glm::vec4& color) {
	setClearColor(color.r, color.g, color.b, color.a);
}

void hyp::RenderCommand::setClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void hyp::RenderCommand::clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void hyp::RenderCommand::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
	glViewport(x, y, width, height);
}

void hyp::RenderCommand::drawIndexed(const hyp::Ref<hyp::VertexArray>& vao, uint32_t indexCount) {
	vao->bind();
	uint32_t count = indexCount ? indexCount : vao->getElementBuffer()->getCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}

void hyp::RenderCommand::drawLines(const hyp::Ref<hyp::VertexArray>& vao, uint32_t vertexCount) {
	vao->bind();
	glDrawArrays(GL_LINES, 0, vertexCount);
}

void hyp::RenderCommand::setLineWidth(float width) {
	glLineWidth(width);
}
