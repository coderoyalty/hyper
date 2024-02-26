#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.hpp"
#include <sol/sol.hpp>
#include "scripting/lua_bindings.hpp"
#include "renderer/vertex_array.hpp"
#include "renderer/vertex_buffer.hpp"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main(int argc, char **argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(600, 600, "C++ Window", nullptr, nullptr);

	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// triangle data
	float vertices[] = {
			// first triangle
			-0.9f, -0.5f, 0.0f, // left
			-0.0f, -0.5f, 0.0f, // right
			-0.45f, 0.5f, 0.0f, // top
													// second triangle
			0.0f, -0.5f, 0.0f,	// left
			0.9f, -0.5f, 0.0f,	// right
			0.45f, 0.5f, 0.0f		// top
	};

	Ref<VertexBuffer> vbo = CreateRef<VertexBuffer>(vertices, sizeof(vertices));
	BufferLayout layout({VertexAttribDescriptor(ShaderDataType::Vec3, "aPos", false)});
	vbo->setLayout(layout);

	Scope<VertexArray> vao = CreateScope<VertexArray>();
	vao->addVertexBuffer(vbo);

	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::package);

	wow::binding::bindEngineClasses(lua);
	lua.script_file(std::string(argc > 1 ? argv[1] : "scripts/color_triangle.lua"));
	sol::function scriptMainFunc = lua["main"];

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		processInput(window);

		// draw the triangle
		scriptMainFunc();
		// glBindVertexArray(VAO);
		vao->bind();
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
};

void framebuffer_size_callback(GLFWwindow *window, int w, int h)
{
	glViewport(0, 0, w, h);
};

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}