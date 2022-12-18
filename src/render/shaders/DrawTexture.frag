// Copyright(c) 2022 Jounayd Id Salah
//
// Refs:
// - https://www.shadertoy.com/view/lslGzl
#version 450
layout(location = 0, binding = 0) uniform sampler2D unTexture;

layout(location = 0) in vec2 inTexCoords;

layout(location = 0) out vec4 outColor;

void main()
{
	outColor.xyz = texture(unTexture, inTexCoords).rgb;
    outColor.w = 1.0f;
}
