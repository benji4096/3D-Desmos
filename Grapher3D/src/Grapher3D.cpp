#include "Grapher3D.hpp"

#include "glm/gtc/matrix_transform.hpp"

Grapher3D::Grapher3D(int windowWidth, int windowHeight)
	: m_glfwContext(), m_window(windowWidth, windowHeight, "3D Desmos"), m_glewContext(),
	m_mesh(&vertices, &indices,
		{
			{ 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, xy) },
			{ 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv) },
		},
		GL_UNSIGNED_BYTE)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	rectShader.addSources({ "res/rect.vert", "res/rect.frag" });
	rectShader.compile();
	rectShader.link();

	m_mesh.upload();
}

Grapher3D::~Grapher3D()
{
}

void Grapher3D::run()
{
	GLuint vao;
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
	glm::mat4 projection = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -5.0f));
	glm::mat4 rotate = glm::mat4(1.0f);
	glm::mat4 transform;
	GLuint tLoc = rectShader.getUniformLocation("transform");

	while (!m_window.shouldClose()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen


		rectShader.use();
		rotate = glm::rotate(rotate, 0.025f, glm::vec3(1.0f, 1.0f, 1.0f));
		transform = projection * translate * rotate * scale;
		rectShader.uniform(tLoc, transform);
		m_mesh.draw();


		m_window.swapBuffers(); // swap window buffers

		glfwPollEvents(); // poll for window events
	}
}
