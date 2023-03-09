
#version 450 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv_point;

out vec2 uv;
out vec3 position;

uniform mat4 transform;

void main()
{
	vec4 outPos = transform * vec4(pos, 1.0f);
	gl_Position = outPos;
	uv = uv_point;
	position = outPos.xyz;
}
