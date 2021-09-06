#version 450
layout(location = 0) in flat uint  inColor;
layout(location = 0) out uint outColor;

void main()
{
    outColor = inColor;
    //outColor = 10;
}
