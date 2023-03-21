#include "Grapher3D.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include <string>

Grapher3D::Grapher3D(int windowWidth, int windowHeight)
	: m_glfwContext(), m_window(windowWidth, windowHeight, "3D Desmos"), m_glewContext(),
	m_mesh(&vertices, &indices,
		{
			{ 0, 3, GL_FLOAT, GL_FALSE, offsetof(Vertex, xy) },	// info on how OpenGL should read vertex position data
			{ 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, uv) }	// info on how OpenGL should read vertex UV data
		},
		GL_UNSIGNED_BYTE), // index type
		testText()
{
  windowResize();

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // background color for OpenGL
	glEnable(GL_DEPTH_TEST); // enable depth testing

	cubeShader.addSources({ "res/shaders/cube.vert", "res/shaders/cube.frag" }); // set the source for the cube shader
	cubeShader.compile(); // compile cube shader
	cubeShader.link(); // link cube shader

	m_mesh.upload(); // upload cube mesh to GPU


	testText.setText(" Hello World!");
	testText.setSize(50);
}

void Grapher3D::windowResize()
{
  m_windowDims = m_window.getWindowDims();
  glViewport(0, 0, m_windowDims.x, m_windowDims.y);
}

void Grapher3D::run()
{
	glm::mat4 projection = glm::perspective(45.0f, float(m_windowDims.x) / float(m_windowDims.y), 0.1f, 100.0f); // perspective transformation
	glm::mat4 center = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, -0.5f)); // translate cube to center
	glm::mat4 rotate = glm::mat4(1.0f); // rotation of cube
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)); // translate cube to center
	glm::mat4 transform; // combined transformation matrix

	GLuint tLoc = cubeShader.getUniformLocation("transform"); // uniform handle for transformation uniform

	while (!m_window.shouldClose())
	{
	  windowResize();
	  
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen



		glEnable(GL_CULL_FACE); // enable culling

		cubeShader.use(); // enable shader for cube
		projection = glm::perspective(45.0f, float(m_windowDims.x) / float(m_windowDims.y), 0.1f, 100.0f); // perspective update aspect ratio
		rotate = glm::rotate(rotate, 0.025f, glm::vec3(1.0f, 1.0f, 0.0f)); // rotation animation
		transform = projection * translate * rotate * center; // combine individual transformations
		cubeShader.uniform(tLoc, transform); // send transformation matrix to GPU
		m_mesh.draw(); // draw cube

		testText.setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		testText.setText(std::to_string(transform[0][0]) + "\n" + "Hello World!");

		testText.draw(m_windowDims); // draw text


		glCheckError(__LINE__); // check for OpenGL errors

		m_window.swapBuffers(); // swap window buffers

		glfwPollEvents(); // poll for window events
	}
}
