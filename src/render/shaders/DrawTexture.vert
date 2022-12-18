// Copyright(c) 2022 Jounayd Id Salah
#version 450
layout(location = 0) out vec2 outTexCoords;

void main()
{
    // We draw a unique triangle that is larger than the viewport because it is faster than two triangles for a fullscreen quad
    // https://stackoverflow.com/questions/2588875/whats-the-best-way-to-draw-a-fullscreen-quad-in-opengl-3-2/51625078
    // https://wallisc.github.io/rendering/2021/04/18/Fullscreen-Pass.html
    // The reason is that there are no diagonals that bound two triangles, which can reduce hardware optimizations.
    vec2 vertices[3] = vec2[3](vec2(-1,-1), vec2(3,-1), vec2(-1, 3));
    gl_Position = vec4(vertices[gl_VertexID],0,1);
    outTexCoords = 0.5 * gl_Position.xy + vec2(0.5);
}
