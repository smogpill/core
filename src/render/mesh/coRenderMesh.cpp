// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/mesh/coRenderMesh.h"
#include "container/array/coDynamicArray_f.h"
#include <lang/result/coResult_f.h>

coResult coRenderMesh::Init()
{
	glGenVertexArrays(1, &vertexArrayObject);
	glGenBuffers(1, &vertexBufferObject);
	glGenBuffers(1, &elementBufferObject);

	return true;
}

void coRenderMesh::Fill(coArray<VertexP>& vertices, const coArray<coVec3>& positions)
{
    coASSERT(vertices.count == positions.count);
    for (coUint i = 0; i < positions.count; ++i)
    {
        VertexP& vertex = vertices[i];
        vertex.SetPos(positions[i]);
    }
}

void coRenderMesh::Fill(coArray<VertexPN>& vertices, const coArray<coVec3>& positions, const coArray<coVec3>& normals)
{
    coASSERT(vertices.count == positions.count);
    coASSERT(vertices.count == normals.count);
    for (coUint i = 0; i < positions.count; ++i)
    {
        VertexPN& vertex = vertices[i];
        vertex.SetPos(positions[i]);
        vertex.SetNormal(normals[i]);
    }
}

void coRenderMesh::SetBuffers(coUint nbVertices_)
{
    drawWithIndices = false;
    nbVertices = nbVertices_;
}

void coRenderMesh::Draw()
{
    glBindVertexArray(vertexArrayObject);
    if (drawWithIndices)
    {
        glDrawElements(GL_TRIANGLES, nbIndices, GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, nbVertices);
    }
    glBindVertexArray(0);
}

void coRenderMesh::DrawAsPoints()
{
    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_POINTS, 0, nbVertices);
    glBindVertexArray(0);
}

void coRenderMesh::VertexP::SetVertexAttribs()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexP), (void*)0);
}

void coRenderMesh::VertexPN::SetVertexAttribs()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPN), (void*)offsetof(VertexPN, normal));
}

void coRenderMesh::VertexPUx2::SetVertexAttribs()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPUx2), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_UNSIGNED_INT, GL_FALSE, sizeof(VertexPUx2), (void*)offsetof(VertexPUx2, u));
}
