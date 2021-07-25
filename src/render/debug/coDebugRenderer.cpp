// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/debug/coDebugRenderer.h"
#include "render/shader/coShader.h"
#include "render/shader/coShaderProgram.h"
#include "lang/result/coResult_f.h"
#include <math/shape/coAabb.h>
#include <math/vector/coVec3_f.h>

coDEFINE_SINGLETON(coDebugRenderer);

coResult coDebugRenderer::Init()
{
	coTRY(InitShaders(), nullptr);
	glGenVertexArrays(1, &vertexArrayObject);
	glGenBuffers(1, &vertexBufferObject);
	glGenBuffers(1, &elementBufferObject);
	return true;
}

coResult coDebugRenderer::InitShaders()
{
	coShader* concreteVS = new coShader();
	{
		coTRY(concreteVS->Init(coShader::Type::VERTEX, "shaders/render/Debug"), nullptr);
		coPushBack(shaders, concreteVS);
	}
	coShader* concreteFS = new coShader();
	{
		coTRY(concreteFS->Init(coShader::Type::FRAGMENT, "shaders/render/Debug"), nullptr);
		coPushBack(shaders, concreteFS);
	}
	shaderProgram = new coShaderProgram();
	coDynamicArray<const coShader*> shaderList;
	coPushBack(shaderList, concreteVS);
	coPushBack(shaderList, concreteFS);
	coTRY(shaderProgram->Init(shaderList), nullptr);
	return true;
}

void coDebugRenderer::DrawLine(const coVec3& a, const coVec3& b, const coColor& color)
{
	coPushBack(lines, Vertex(a, color));
	coPushBack(lines, Vertex(b, color));
}

void coDebugRenderer::DrawTriangle(const coVec3& a, const coVec3& b, const coVec3& c, const coColor& color)
{
	coPushBack(lines, Vertex(a, color));
	coPushBack(lines, Vertex(b, color));
	coPushBack(lines, Vertex(c, color));
}

void coDebugRenderer::Draw(const coAabb& aabb, const coColor& color)
{

}

void coDebugRenderer::DrawWireframe(const coAabb& aabb, const coColor& color)
{
	coReserve(lines, 12);

	const coVec3 size = aabb.max - aabb.min;
	const coVec3 sx = aabb.min + coVec3(size.x, 0, 0);
	const coVec3 sy = aabb.min + coVec3(0, size.y, 0);
	const coVec3 sz = aabb.min + coVec3(0, 0, size.z);

	const coVec3 a(aabb.min);
	const coVec3 b(aabb.min + sx);
	const coVec3 c(aabb.min + sx + sy);
	const coVec3 d(aabb.min + sy);
	const coVec3 e(a + sz);
	const coVec3 f(b + sz);
	const coVec3 g(c + sz);
	const coVec3 h(d + sz);

	DrawLine(a, b, color);
	DrawLine(b, c, color);
	DrawLine(c, d, color);
	DrawLine(d, a, color);

	DrawLine(e, f, color);
	DrawLine(f, g, color);
	DrawLine(g, h, color);
	DrawLine(h, e, color);

	DrawLine(a, e, color);
	DrawLine(b, f, color);
	DrawLine(c, g, color);
	DrawLine(d, h, color);
}

void coDebugRenderer::Render(const coMat4& viewProj)
{
	shaderProgram->Bind();

	const auto modelViewProjLocation = shaderProgram->GetUniformLocation("modelViewProj");
	shaderProgram->SetUniform(modelViewProjLocation, viewProj);

	glBindVertexArray(vertexArrayObject);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, lines.count * sizeof(Vertex), lines.data, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
	glEnableVertexAttribArray(1);
	glVertexAttribIPointer(1, 4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glDrawArrays(GL_LINES, 0, lines.count);

	glBindVertexArray(0);
	shaderProgram->Unbind();
}
