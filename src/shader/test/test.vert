#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject
{
    mat4 modelViewProj;
} ubo;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec2 inUv;

out gl_PerVertex
{
    vec4 gl_Position;
};

layout(location = 0) out vec3 fragColor;

void main()
{
    gl_Position = ubo.modelViewProj * vec4(inPos, 1.0f);
    //fragColor = vec3(dot(inNormal, normalize(vec3(1, 1, 0))));
	fragColor = abs(inNormal);
}
