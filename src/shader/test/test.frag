#version 450

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec3 inNormal;
layout(location = 0) out vec4 outColor;

void main()
{
	float diffuse = dot(normalize(inNormal), normalize(vec3(-2, -2, -1)));
    outColor = vec4(vec3(diffuse), 1.0);
}
