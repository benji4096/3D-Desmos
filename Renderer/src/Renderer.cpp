#include "Renderer.hpp"

#include <stdexcept>
#include <string>
#include <iostream>

#include "glUtils.h"

GLEWContext::GLEWContext()
{
	//glewExperimental = GL_TRUE;
	GLenum err = glewInit(); // initalize GLEW
	if (err != GLEW_OK) // GLEW failed to initalize
		throw std::runtime_error(std::string("Could not initalize GLEW (") + (const char*)glewGetErrorString(err) + ')');
	if (!glewIsSupported("GL_ARB_direct_state_access"))
		throw std::runtime_error("Modern OpenGL unsupported (extension=ARB_direct_state_access)");

	/*GLint extensionCount;
	glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount);
	std::cout << "Number of supported extensions: " << extensionCount << std::endl;
	for (int i = 0; i < extensionCount; i++)
	{
		std::cout << glGetStringi(GL_EXTENSIONS, i) << std::endl;
	}*/
}

GLEWContext::~GLEWContext()
{
}

GLFWContext::GLFWContext()
{
	if (!glfwInit()) // initalize GLFW
		throw std::runtime_error("Could not initalize GLFW");
}

GLFWContext::~GLFWContext()
{
	glfwTerminate(); // terminate GLFW
}

Window::Window(int width, int height, const char* name)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);

	m_window = glfwCreateWindow(width, height, name, NULL, NULL); // create new GLFW window
	if (!m_window) { // failed to create window
		throw std::runtime_error("Could not create GLFW window");
	}

	makeContextCurrent();

	glfwSwapInterval(1);
}

Window::~Window()
{
	if (m_window)
		glfwDestroyWindow(m_window); // destroy the GLFW window
}

void Window::makeContextCurrent()
{
	glfwMakeContextCurrent(m_window);
}

bool Window::shouldClose()
{
	return glfwWindowShouldClose(m_window);;
}

void Window::swapBuffers()
{
	glfwSwapBuffers(m_window);
}
