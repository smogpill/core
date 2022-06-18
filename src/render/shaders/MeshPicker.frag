#version 450
layout(location = 1) uniform uint id;
layout(location = 0) out uint outColor;

void main()
{
    outColor = id;
}
