#version 450
layout(location = 0) uniform mat4 modelViewProj;
layout(location = 1) uniform uvec2 id;
layout(location = 0) in vec3 inPos;
layout(location = 0) out uint outColor;

void main()
{
    gl_Position = modelViewProj * vec4(inPos, 1);
    outColor = gl_VertexID + 1;
}
