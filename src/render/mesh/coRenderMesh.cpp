// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/mesh/coRenderMesh.h"
#include "container/array/coDynamicArray_f.h"
#include <lang/result/coResult_f.h>
#include "../context/coRenderContext.h"

coRenderMesh::coRenderMesh()
{
    glCreateVertexArrays(1, &vertexArrayObject);
    glCreateBuffers(1, &vertexBufferObject);
    glCreateBuffers(1, &elementBufferObject);
}

coRenderMesh::~coRenderMesh()
{
    glDeleteBuffers(1, &vertexBufferObject);
    glDeleteBuffers(1, &elementBufferObject);
    glDeleteVertexArrays(1, &vertexArrayObject);
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

void coRenderMesh::Draw() const
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

void coRenderMesh::DrawAsPoints() const
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

void coRenderMesh::VertexPC::SetVertexAttribs()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexPC), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexPC), (void*)offsetof(VertexPC, color));
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

void coRenderMesh::SetDebugLabel(const coConstString& label)
{
    coRenderContext::SetGLDebugLabel(GL_VERTEX_ARRAY, vertexArrayObject, label);
    coRenderContext::SetGLDebugLabel(GL_BUFFER, vertexBufferObject, label);
    coRenderContext::SetGLDebugLabel(GL_BUFFER, elementBufferObject, label);
}