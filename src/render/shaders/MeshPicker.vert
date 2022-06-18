#version 450
layout(location = 0) uniform mat4 modelViewProj;
layout(location = 0) in vec3 inPos;

void main()
{
    gl_Position = modelViewProj * vec4(inPos, 1);
}
