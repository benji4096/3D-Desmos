
#version 450 core

out vec4 FragColor;

in vec2 uv;
in vec3 position;

void main()
{
	FragColor = vec4(uv, 0.0f, 1.0);
}
