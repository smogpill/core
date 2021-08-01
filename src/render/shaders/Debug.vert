#version 450
layout(location = 0) uniform mat4 modelViewProj;
layout(location = 0) in vec3 inPos;
layout(location = 1) in uvec4 inColor;
layout(location = 0) out vec4 outColor;

void main()
{
    gl_Position = modelViewProj * vec4(inPos, 1);
    outColor = vec4(inColor) / 255.0f;
}
