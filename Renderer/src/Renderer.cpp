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
	std::cerr << "GLEW initalized succesfully" << std::endl;
}

GLEWContext::~GLEWContext()
{
}

GLFWContext::GLFWContext()
{
	if (!glfwInit()) // initalize GLFW
		throw std::runtime_error("Could not initalize GLFW");

	std::cerr << "GLFW initalized succesfully" << std::endl;
}

GLFWContext::~GLFWContext()
{
	glfwTerminate(); // terminate GLFW

	std::cerr << "GLFW terminated succesfully" << std::endl;
}

Window::Window(int width, int height, const char* name)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

	m_window = glfwCreateWindow(width, height, name, NULL, NULL); // create new GLFW window
	if (!m_window) { // failed to create window
		throw std::runtime_error("Could not create GLFW window");
	}

	makeContextCurrent();

	glfwSwapInterval(1);

	std::cerr << "Window initalized succesfully" << std::endl;
}

Window::~Window()
{
	if (m_window)
		glfwDestroyWindow(m_window); // destroy the GLFW window

	std::cerr << "Window destroyed succesfully" << std::endl;
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

glm::ivec2 Window::getWindowDims()
{
  glm::ivec2 rtn;
  glfwGetWindowSize(m_window, &rtn.x, &rtn.y);
  return rtn;
}
glm::vec2 Window::getMousePos()
{
  double x, y;
  glfwGetCursorPos(m_window, &x, &y);
  return { (float)x, (float)y };
}
