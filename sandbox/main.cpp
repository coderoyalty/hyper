#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <renderer/shader.hpp>
#include <renderer/vertex_array.hpp>
#include <renderer/vertex_buffer.hpp>
#include <utils/logger.hpp>
#include <core/device.hpp>
#include <core/window.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main(int argc, char** argv)
{
	hyp::Device::init({});
	auto applicationWindow = hyp::Window::createWindow({});

	auto window = applicationWindow->getRawWindow();

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// triangle data
	float vertices[] = {
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f    //
	};

	hyp::Ref<hyp::VertexBuffer> vbo = hyp::CreateRef<hyp::VertexBuffer>(
		vertices, sizeof(vertices));
	hyp::BufferLayout layout({
	   hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec3, "aPos", false),
	   hyp::VertexAttribDescriptor(hyp::ShaderDataType::Vec3, "aColor", false)
		});
	vbo->setLayout(layout);

	hyp::Scope<hyp::VertexArray> vao = hyp::CreateScope<hyp::VertexArray>();
	vao->addVertexBuffer(vbo);

	hyp::ShaderProgram program("assets/vertex.vert", "assets/fragment.frag");

	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	program.link();
	program.use();
	program.setFloat("redValue", 1.0);
	program.setMat4("transform", trans);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);

		vao->bind();
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	hyp::Device::deinit();
	return 0;
};

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
};

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}
