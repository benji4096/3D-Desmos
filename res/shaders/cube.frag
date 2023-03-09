
#version 450 core

out vec4 FragColor;

in vec2 uv;
in vec3 position;

void main()
{
	FragColor = vec4(cross(vec3(uv, 1.0f), position), 1.0);
}
