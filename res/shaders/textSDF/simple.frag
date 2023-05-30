
#version 450 core

out vec4 FragColor;

in vec2 uv;

uniform sampler2D tex;

uniform vec4 textColor;

uniform float aaDist;

float distMin = 0.5 - aaDist;
float distMax = 0.5 + aaDist;

//#define positionDebug

//#define debugAA

void main()
{
#ifndef positionDebug
	float dist = 1.0f - texture(tex, uv).r;

	if (dist > distMax)
		discard;

#ifndef debugAA

	FragColor = textColor * vec4(vec3(1.0f), clamp((distMax - dist) / (distMax - distMin), 0, 1));

#else

	FragColor = vec4(vec3(1.0f), clamp((distMax - dist) / (distMax - distMin), 0, 1));
	if (dist < 0.5)
		FragColor = vec4(vec3(1.0f, 0.0f, 0.0f), 1.0f);

#endif
#else

	FragColor = vec4(uv, 0.0f, 0.5f);
#endif
}
