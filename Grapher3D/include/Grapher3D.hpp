#pragma once

#include "Renderer.hpp"

class Grapher3D
{
	GLFWContext m_glfwContext; // container responsible for initalizing GLFW
	Window m_window; // GLFW window container
	GLEWContext m_glewContext; // container responsible for initalizing GLEW

public:
	Grapher3D(int windowWidth = 900, int windowHeight = 600);
	Grapher3D(const Grapher3D& other) = delete;
	const Grapher3D& operator=(const Grapher3D& other) = delete;
	~Grapher3D();

	void run(); // main program loop
};

