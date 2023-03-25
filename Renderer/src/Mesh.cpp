#include "Mesh.hpp"

#include <optional>

#include "glm/glm.hpp"

IMeshStandard::IMeshStandard(std::vector<StandardVertex> const* verts, std::vector<uint32_t> const* indices)
	: IMeshUniversal(verts, indices, 
		{
			{ 0, 3, GL_FLOAT, GL_FALSE, offsetof(StandardVertex, p) },	// info on how OpenGL should read vertex position data
			{ 1, 2, GL_FLOAT, GL_FALSE, offsetof(StandardVertex, n) }	// info on how OpenGL should read vertex normal data
		},
		GL_UNSIGNED_INT)
{
}
