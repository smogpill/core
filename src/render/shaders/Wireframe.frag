// Copyright(c) 2022 Jounayd Id Salah
#version 450
layout(location = 3) uniform vec3 wireframeColor = vec3(0);
layout(location = 4) uniform float wireframeSmoothing = 0.5;
layout(location = 5) uniform float wireframeThickness = 0.3;
layout(location = 6) uniform float wireframeFadeDistance = 20;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inBary;
layout(location = 3) in float inDist;

layout(location = 0) out vec4 outColor;

void main()
{
    vec3 deltas = fwidth(inBary);
    vec3 smoothing = deltas * wireframeSmoothing;
	vec3 thickness = deltas * wireframeThickness;
	vec3 bary = smoothstep(thickness, thickness + smoothing, inBary);
    float distFactor = min(inDist / wireframeFadeDistance, 1);
    distFactor = distFactor * distFactor;
	float factor = mix(min(bary.x, min(bary.y, bary.z)), 1, distFactor);
    if (factor == 1.0f)
    {
        discard;
        return;
    }
    outColor.xyz = wireframeColor;
    outColor.w = 1.0f - factor;
}
