#include "glUtils.h"

#include <GL/glew.h>
#include <string>
#include <iostream>

void glCheckError(uint32_t line)
{
	uint32_t err = glGetError();
	//std::cout << err << " " <<  << " " << (glGetError() != GL_NO_ERROR) << std::endl;

	std::string errorString;
	switch (err)
	{
	case 1280:
	{ errorString = "GL_INVALID_ENUM"; break; }
	case 1281:
	{ errorString = "GL_INVALID_VALUE"; break; }
	case 1282:
	{ errorString = "GL_INVALID_OPERATION"; break; }
	case 1283:
	{ errorString = "GL_STACK_OVERFLOW"; break; }
	case 1284:
	{ errorString = "GL_STACK_UNDERFLOW"; break; }
	case 1285:
	{ errorString = "GL_OUT_OF_MEMORY"; break; }
	case 1286:
	{ errorString = "GL_INVALID_FRAMEBUFFER_OPERATION"; break; }
	}

	if (err != GL_NO_ERROR)
		std::cerr << "OpenGL Error at line " << line << ": " << err << " (" << errorString << ")" << std::endl;
}
