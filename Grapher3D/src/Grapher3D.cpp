#include "Grapher3D.hpp"

Grapher3D::Grapher3D(int windowWidth, int windowHeight)
	: m_glfwContext(), m_window(windowWidth, windowHeight, "3D Desmos"), m_glewContext()
{
}

Grapher3D::~Grapher3D()
{
}

void Grapher3D::run()
{
	while (!m_window.shouldClose()) {
		glClear(GL_COLOR_BUFFER_BIT); // clear screen

		m_window.swapBuffers(); // swap window buffers

		glfwPollEvents(); // poll for window events
	}
}
