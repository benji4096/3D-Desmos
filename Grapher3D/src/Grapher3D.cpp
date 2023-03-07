#include "Grapher3D.hpp"

Grapher3D::Grapher3D(int windowWidth, int windowHeight)
	: m_glfwContext(), m_window(windowWidth, windowHeight, "3D Desmos"), m_glewContext()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	simpleShader.addSources({ "res/test.vert", "res/test.frag" });
	simpleShader.compile();
	simpleShader.link();
}

Grapher3D::~Grapher3D()
{
}

void Grapher3D::run()
{
	GLuint vao;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	GLuint offsetUniformLocation = simpleShader.getUniformLocation("offset");

	float t = 0.0f;

	while (!m_window.shouldClose()) {
		glClear(GL_COLOR_BUFFER_BIT); // clear screen


		t += 0.01f;
		simpleShader.uniform(offsetUniformLocation, 0.3f * sin(t));
		simpleShader.use();
		glCheckError(__LINE__);
		glPointSize(30.0f);
		glCheckError(__LINE__);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glCheckError(__LINE__);


		m_window.swapBuffers(); // swap window buffers

		glfwPollEvents(); // poll for window events
	}
}
