#version 450
layout(location = 0) out uint outColor;

void main()
{
    outColor = gl_PrimitiveID + 1;
}
