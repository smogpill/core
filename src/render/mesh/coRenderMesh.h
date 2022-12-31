// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/vector/coVec3.h"
#include "math/vector/coColor.h"
#include "lang/result/coResult.h"

template <class T> class coArray;
template <class T> class coDynamicArray;

class coRenderMesh
{
public:
	struct VertexP
	{
		void SetPos(const coVec3& v) { pos[0] = v.x; pos[1] = v.y; pos[2] = v.z; }
		static void SetVertexAttribs();
		coFloat pos[3];
	};
	struct VertexPC : public VertexP
	{
		void SetColor(const coColor& c) { color = c; }
		static void SetVertexAttribs();
		coUint32 color;
	};
	struct VertexPN : public VertexP
	{
		void SetNormal(const coVec3& v) { normal[0] = v.x; normal[1] = v.y; normal[2] = v.z; }
		static void SetVertexAttribs();
		coFloat normal[3];
	};
	struct VertexPUx2 : public VertexP
	{
		void SetU2(const coUint64 v) { u[0] = coUint32(v); u[1] = coUint32(v >> 32); }
		static void SetVertexAttribs();
		coUint32 u[2];
	};

	coRenderMesh();
	~coRenderMesh();
	static void Fill(coArray<VertexP>& vertices, const coArray<coVec3>& positions);
	static void Fill(coArray<VertexPN>& vertices, const coArray<coVec3>& positions, const coArray<coVec3>& normals);
	template <class T>
	void SetBuffers(const coArray<T>& vertices);
	template <class T>
	void SetBuffers(const coArray<T>& vertices, const coArray<coUint32>& indices);
	void SetBuffers(coUint nbVertices);
	void SetDebugLabel(const coConstString& label);
	void Draw() const;
	void DrawAsPoints() const;

private:
	GLuint vertexArrayObject = 0;
	GLuint vertexBufferObject = 0;
	GLuint elementBufferObject = 0;
	coBool drawWithIndices = 0;
	coUint nbVertices = 0;
	coUint nbIndices = 0;
};

template <class T>
void coRenderMesh::SetBuffers(const coArray<T>& vertices)
{
	drawWithIndices = false;
	SetBuffers(vertices, coArray<coUint32>(nullptr, 0));
}

template <class T>
void coRenderMesh::SetBuffers(const coArray<T>& vertices, const coArray<coUint32>& indices)
{
	drawWithIndices = true;
	nbVertices = vertices.count;
	nbIndices = indices.count;
	glBindVertexArray(vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, vertices.count * sizeof(VertexPN), vertices.data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.count * sizeof(coUint32), indices.data, GL_STATIC_DRAW);

	T::SetVertexAttribs();

	glBindVertexArray(0);
}