#version 450

layout(triangles) in;
layout(location = 0) in vec3 inPos[];
layout(location = 1) in vec3 inNormal[];
layout(location = 2) in float inDist[];

layout(triangle_strip, max_vertices = 15) out;
layout(location = 0) out vec3 outPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outBarycenter;
layout(location = 3) out float outDist;

void main()
{
	vec3 normal = cross(inPos[2] - inPos[0], inPos[1] - inPos[0]);

	// Triangle
	for (int i = 0; i < 3; ++i)
	{
		outPos = inPos[i];
		outNormal = inNormal[i];
		//outNormal = normal;
		outBarycenter = vec3(0);
		outBarycenter[i] = 1;
		outDist = inDist[i];
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}