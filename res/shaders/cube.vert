
#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 norm;
out vec3 pos;

uniform mat4 transform;
uniform mat3 normal_transform;

void main()
{
	vec4 outPos = transform * vec4(position, 1.0f);
	gl_Position = outPos;
	norm = normalize(normal_transform * normal);
	//norm = normal;
	pos = outPos.xyz;
}
