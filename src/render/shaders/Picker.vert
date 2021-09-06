#version 450
layout(location = 0) uniform mat4 modelViewProj;
layout(location = 1) uniform uvec2 id;
layout(location = 0) in vec3 inPos;
layout(location = 0) out uvec4 outColor;

void main()
{
    gl_Position = modelViewProj * vec4(inPos, 1);
    outColor.xy = id.xy;
    outColor.w = gl_VertexID + 1;
}
