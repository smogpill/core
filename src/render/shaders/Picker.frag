#version 450
layout(location = 0) in flat uvec4  inColor;
layout(location = 0) out uvec4 outColor;

void main()
{
    outColor = inColor;
}
