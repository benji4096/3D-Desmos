#include "Grapher3D.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include <string>

#include "MathExpression.hpp"

Grapher3D::Grapher3D(int windowWidth, int windowHeight)
	: m_glfwContext(), m_window(windowWidth, windowHeight, "3D Desmos"), m_glewContext(),
		m_mesh2(&vertices2, &indices2), m_mesh(),
		testText()
{
  windowResize();

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // background color for OpenGL
	glEnable(GL_DEPTH_TEST); // enable depth testing

	cubeShader.addSources({ "res/shaders/cube.vert", "res/shaders/cube.frag" }); // set the source for the cube shader
	cubeShader.compile(); // compile cube shader
	cubeShader.link(); // link cube shader

	m_mesh2.upload(); // upload cube mesh to GPU

	testText.setText(" Hello World!");
	testText.setSize(50);


	std::unordered_map<std::string, float> vars = std::unordered_map<std::string, float>();
	vars.insert({ "x", 5});
	MathExpression expr = MathExpression();
	testText.setText(std::to_string(expr.eval(vars)));


	const int numSegments = 1000;
	for (int i = 0; i < numSegments + 1; i++)
	{
		float v = float(i) / float(numSegments);
		v = 3 * (v - 0.5f);
		//   0-2-4-6-8
		//   |\|\|\|\|
		//   1-3-5-7-9

		glm::vec3 n = {-2.0 * v, 1, 0};

		n = glm::normalize(n);
		m_mesh.addVertex( { v * v, v, 0.0f }, n);
		m_mesh.addVertex( { v * v, v, 1.0f }, n);
	}
	for (int i = 0; i < numSegments; i++)
	{
		int index = i * 2;
		m_mesh.addTri( { index, index + 1, index + 3 } );
		m_mesh.addTri( { index, index + 3, index + 2 } );
	}
	m_mesh.upload();
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
	glm::mat4 model; // combined model transform
	glm::mat3 normalTransform; // transformations for object normal
	glm::mat4 transform; // combined transformation matrix

	GLuint tLoc = cubeShader.getUniformLocation("transform"); // uniform handle for transformation uniform
	GLuint ntLoc = cubeShader.getUniformLocation("normal_transform"); // uniform handle for transformation uniform

	while (!m_window.shouldClose())
	{
		windowResize();
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen



		//glEnable(GL_CULL_FACE); // enable culling

		cubeShader.use(); // enable shader for cube
		projection = glm::perspective(45.0f, float(m_windowDims.x) / float(m_windowDims.y), 0.1f, 100.0f); // perspective update aspect ratio
		rotate = glm::rotate(rotate, 0.025f, glm::vec3(1.0f, 1.0f, 0.0f)); // rotation animation
		model = translate * rotate * center;
		normalTransform = glm::mat3(glm::transpose(glm::inverse(model)));
		transform = projection * model; // combine individual transformations
		cubeShader.uniform(tLoc, transform); // send transformation matrix to GPU
		cubeShader.uniform(ntLoc, normalTransform); // send normal transformation matrix to GPU
		//m_mesh2.draw(); // draw cube
		m_mesh.draw(); // draw parabola

		//testText.setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		//testText.setText(std::to_string(transform[0][0]) + "\n" + "Hello World!");

		testText.draw(m_windowDims); // draw text


		glCheckError(__LINE__); // check for OpenGL errors

		m_window.swapBuffers(); // swap window buffers

		glfwPollEvents(); // poll for window events
	}
}
