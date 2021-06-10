// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/vector/coVec3.h"
#include "lang/result/coResult.h"

template <class T> class coArray;

class coRenderMesh
{
public:
	struct Vertex
	{
		coFloat pos[3];
		coFloat normal[3];
	};

	coResult Init();

	void SetBuffers(const coArray<Vertex>& vertices, const coArray<coUint32>& indices);
	void SetBuffers(const coArray<coVec3>& positions, const coArray<coVec3>& normals, const coArray<coUint32>& indices);
	void Draw();

private:
	GLuint vertexArrayObject = 0;
	GLuint vertexBufferObject = 0;
	GLuint elementBufferObject = 0;
	coUint nbIndices = 0;
};
