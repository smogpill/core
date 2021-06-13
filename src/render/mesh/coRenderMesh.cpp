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

void coRenderMesh::SetBuffers(const coArray<coVec3>& positions, const coArray<coVec3>& normals, const coArray<coUint32>& indices)
{
    coASSERT(positions.count == normals.count);
    coDynamicArray<Vertex> vertices;
    coResize(vertices, positions.count);
    for (coUint i = 0; i < positions.count; ++i)
    {
        Vertex& vertex = vertices[i];
        const coVec3& pos = positions[i];
        const coVec3& normal = normals[i];
        vertex.pos[0] = pos.x;
        vertex.pos[1] = pos.y;
        vertex.pos[2] = pos.z;
        vertex.normal[0] = normal.x;
        vertex.normal[1] = normal.y;
        vertex.normal[2] = normal.z;
    }
    SetBuffers(vertices, indices);
}

void coRenderMesh::SetBuffers(const coArray<Vertex>& vertices, const coArray<coUint32>& indices)
{
    glBindVertexArray(vertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.count * sizeof(Vertex), vertices.data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.count * sizeof(coUint32), indices.data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));

    glBindVertexArray(0);
    nbIndices = indices.count;
}

void coRenderMesh::Draw()
{
    glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, nbIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
