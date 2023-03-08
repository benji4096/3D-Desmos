#pragma once

#include <optional>

#include "glm/glm.hpp"

#include "Renderer.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"


class Grapher3D
{
	GLFWContext m_glfwContext; // container responsible for initalizing GLFW
	Window m_window; // GLFW window container
	GLEWContext m_glewContext; // container responsible for initalizing GLEW



	Shader rectShader;

	struct Vertex
	{
		glm::vec3 xy;
		glm::vec2 uv;
	};

	std::vector<Vertex> vertices = {
			   { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f } },
			   { { 1.0f, 0.0f, 0.0f }, { 0.333f, 0.0f } },
			   { { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f } },
			   { { 1.0f, 1.0f, 0.0f }, { 0.667f, 0.0f } },

			   { { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },
			   { { 1.0f, 0.0f, 1.0f }, { 0.333f, 1.0f } },
			   { { 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },
			   { { 1.0f, 1.0f, 1.0f }, { 0.667f, 1.0f } }
	};

	std::vector<uint8_t> indices = {
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

	IMeshUniversal<Vertex, uint8_t> m_mesh;

public:
	Grapher3D(int windowWidth = 900, int windowHeight = 600);
	Grapher3D(const Grapher3D& other) = delete;
	const Grapher3D& operator=(const Grapher3D& other) = delete;
	~Grapher3D();

	void run(); // main program loop
};

