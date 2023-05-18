
#version 450 core

out vec4 FragColor;

in vec3 norm;
in vec3 pos;

void main()
{
	vec3 ambient = vec3(0.2);

	vec3 lightPos = vec3(0, 999, 0);
	vec3 lightDir = normalize(lightPos - pos);
	float diffuse = max(dot(norm, lightDir), 0.0);

	FragColor = vec4(vec3(diffuse) + ambient, 1.0);
	//FragColor = vec4(norm, 1.0);
}
