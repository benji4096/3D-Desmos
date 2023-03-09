
#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv_point;

out vec2 uv;
out vec3 position;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(pos, 1.0f);
	uv = uv_point;
	position = pos;
}
