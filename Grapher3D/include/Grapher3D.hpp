#pragma once

#include <optional>

#include "glm/glm.hpp"

#include "Renderer.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

#include "gui.hpp"

class Grapher3D
{
	GLFWContext m_glfwContext; // container responsible for initalizing GLFW
	Window m_window; // GLFW window container
	GLEWContext m_glewContext; // container responsible for initalizing GLEW

  void windowResize();
  glm::ivec2 m_windowDims;
  

	Shader cubeShader;

	std::vector<StandardVertex> vertices2 = { // vertices of a cube
			   { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
			   { { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
			   { { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },
			   { { 1.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } },

			   { { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
			   { { 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
			   { { 0.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } },
			   { { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } }
	};

	std::vector<uint32_t> indices2 = { // triangles that form cube
			0, 3, 1,
			0, 2, 3,
			4, 5, 7,
			4, 7, 6,

			2, 0, 4,
			2, 4, 6,

			0, 1, 5,
			0, 5, 4,

			1, 3, 7,
			1, 7, 5,

			3, 2, 6,
			3, 6, 7
	};

	IMeshStandard m_mesh2;
	MeshBuilder m_mesh;

	TextSDF testText;

public:
	Grapher3D(int windowWidth = 900, int windowHeight = 600);
	Grapher3D(const Grapher3D& other) = delete;
	const Grapher3D& operator=(const Grapher3D& other) = delete;

	void run(); // main program loop
};

