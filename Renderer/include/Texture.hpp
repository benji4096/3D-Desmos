#pragma once

#include "Renderer.hpp"

#include "glm/glm.hpp"

class Texture2D
{
	GLuint id;

	glm::ivec2 mSize;
	bool mMipmap;

public:
	//Texture2D(std::string path, bool mipmap = true);
	Texture2D(unsigned char* data, glm::ivec2 size, GLenum format = GL_RGBA, bool mipmap = true);
	~Texture2D();

	void updateData(unsigned char* data, glm::ivec2 size, GLenum format = GL_RGBA);

	GLuint get();
};
