#version 450 core

layout(push_constant) uniform PushConstants
{
    vec2 scale;
    vec2 translation;
} pc;

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inColor;

out gl_PerVertex
{
    vec4 gl_Position;
};

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outUV;

void main()
{
	outColor = inColor;
	outUV = inUV;
    //gl_Position = vec4(inPos * pc.scale + pc.translation, 0, 1);
	gl_Position = vec4(inPos + vec2(-1, -1), 0, 1);
}
