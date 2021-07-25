#version 450
layout(location = 0) uniform mat4 modelViewProj;
layout(location = 0) in vec3 inPos;
layout(location = 1) in uvec4 inColor;
layout(location = 0) out vec4 outColor;

vec4 ConvertColor(uvec4 c)
{
    return vec4(c);
}

void main()
{
    gl_Position = modelViewProj * vec4(inPos, 1);
    outColor = ConvertColor(inColor);
}
 