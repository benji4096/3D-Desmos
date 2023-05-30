#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

class GLEWContext // container responsible for initalizing GLEW
{
public:
	GLEWContext();
	GLEWContext(const GLEWContext& other) = delete;
	const GLEWContext& operator=(const GLEWContext& other) = delete;
	~GLEWContext();
};

class GLFWContext // container responsible for initalizing GLFW
{
public:
	GLFWContext();
	GLFWContext(const GLFWContext& other) = delete;
	const GLFWContext& operator=(const GLFWContext& other) = delete;
	~GLFWContext();
};

class Window // GLFW window container
{
	GLFWwindow* m_window;

public:
	Window(int width, int height, const char* name);
	Window(const Window& other) = delete;
	const Window& operator=(const Window& other) = delete;
	~Window();

	void makeContextCurrent(); // makes GLFW context current to the current thread
	bool shouldClose(); // if the window should be closed from inputs
	void swapBuffers(); // swap window buffers

  glm::ivec2 getWindowDims();
  	glm::vec2 getMousePos();
	
};


