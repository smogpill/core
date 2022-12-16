#version 450

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 modelView;
layout(location = 2) uniform mat4 modelViewProj;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out float outDist;

void main()
{
    gl_Position = modelViewProj * vec4(inPos, 1);
    outPos = (model * vec4(inPos, 1)).xyz;
    outNormal = (model * vec4(inNormal, 0)).xyz;
    outDist = length((modelView * vec4(inPos, 1)).xyz);
}
