// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/debug/coDebugRenderer.h"
#include "render/shader/coShader.h"
#include "render/shader/coShaderProgram.h"
#include "lang/result/coResult_f.h"
#include <math/shape/coAabb.h>
#include <math/vector/coVec3_f.h>
#include <math/vector/coVec4_f.h>
#include <math/matrix/coMat4_f.h>

coDEFINE_SINGLETON(coDebugRenderer);

coDebugRenderer::~coDebugRenderer()
{
	delete shaderProgram;
	for (coShader* shader : shaders)
		delete shader;
}

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
	coDynamicArray<coShader*> shaderList;
	coPushBack(shaderList, concreteVS);
	coPushBack(shaderList, concreteFS);
	coTRY(shaderProgram->Init(shaderList), nullptr);
	return true;
}

void coDebugRenderer::DrawLine(const coVec3& a, const coVec3& b, const coColor& color, coUint32 options)
{
	auto& buffer = vertexBuffers[Buffer::LINES | (options & Options::NO_DEPTH_TEST)];
	coPushBack(buffer, Vertex(a, color));
	coPushBack(buffer, Vertex(b, color));
}

void coDebugRenderer::DrawTriangle(const coVec3& a, const coVec3& b, const coVec3& c, const coColor& color, coUint32 options)
{
	if (options & WIREFRAME)
	{
		auto& buffer = vertexBuffers[Buffer::LINES | (options & Options::NO_DEPTH_TEST)];
		coPushBack(buffer, Vertex(a, color));
		coPushBack(buffer, Vertex(b, color));
		coPushBack(buffer, Vertex(b, color));
		coPushBack(buffer, Vertex(c, color));
		coPushBack(buffer, Vertex(c, color));
		coPushBack(buffer, Vertex(a, color));
	}
	else
	{
		auto& buffer = vertexBuffers[Buffer::TRIANGLES | (options & Options::NO_DEPTH_TEST)];
		coPushBack(buffer, Vertex(a, color));
		coPushBack(buffer, Vertex(b, color));
		coPushBack(buffer, Vertex(c, color));
	}
}

void coDebugRenderer::Draw(const coAabb& aabb, const coColor& color, coUint32 options)
{
	//coReserve(triangles, triangles.count + 12 * 3);

	const coVec3 size = aabb.max - aabb.min;
	const coVec3 sx = coVec3(size.x, 0, 0);
	const coVec3 sy = coVec3(0, size.y, 0);
	const coVec3 sz = coVec3(0, 0, size.z);

	const coVec3 a(aabb.min);
	const coVec3 b(aabb.min + sx);
	const coVec3 c(aabb.min + sx + sy);
	const coVec3 d(aabb.min + sy);
	const coVec3 e(a + sz);
	const coVec3 f(b + sz);
	const coVec3 g(c + sz);
	const coVec3 h(d + sz);

	if (options & WIREFRAME)
	{
		DrawLine(a, b, color, options);
		DrawLine(b, c, color, options);
		DrawLine(c, d, color, options);
		DrawLine(d, a, color, options);

		DrawLine(e, f, color, options);
		DrawLine(f, g, color, options);
		DrawLine(g, h, color, options);
		DrawLine(h, e, color, options);

		DrawLine(a, e, color, options);
		DrawLine(b, f, color, options);
		DrawLine(c, g, color, options);
		DrawLine(d, h, color, options);
	}
	else
	{
		DrawTriangle(a, b, f, color, options);
		DrawTriangle(a, f, e, color, options);

		DrawTriangle(b, c, g, color, options);
		DrawTriangle(b, g, f, color, options);

		DrawTriangle(c, d, h, color, options);
		DrawTriangle(c, h, g, color, options);

		DrawTriangle(d, a, e, color, options);
		DrawTriangle(d, e, h, color, options);

		// Bottom
		DrawTriangle(b, a, c, color, options);
		DrawTriangle(d, c, a, color, options);

		// Top
		DrawTriangle(e, f, g, color, options);
		DrawTriangle(e, g, h, color, options);
	}
}

void coDebugRenderer::DrawFrame(const coMat4& frame, coFloat size, coUint32 options)
{
	DrawLine(coVec3(frame.c3), coVec3(frame.c3 + frame.c0 * size), coColor::s_coolRed, options);
	DrawLine(coVec3(frame.c3), coVec3(frame.c3 + frame.c1 * size), coColor::s_coolGreen, options);
	DrawLine(coVec3(frame.c3), coVec3(frame.c3 + frame.c2 * size), coColor::s_coolBlue, options);
}

void coDebugRenderer::Render(const coMat4& viewProj)
{
	shaderProgram->Bind();

	const auto modelViewProjLocation = shaderProgram->GetUniformLocation("modelViewProj");
	shaderProgram->SetUniform(modelViewProjLocation, viewProj);

	glBindVertexArray(vertexArrayObject);

	auto drawBuffer = [&](Buffer buffer, GLenum mode)
	{
		auto& vertexBuffer = vertexBuffers[buffer];
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer.count * sizeof(Vertex), vertexBuffer.data, GL_STREAM_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
		glEnableVertexAttribArray(1);
		glVertexAttribIPointer(1, 4, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		glDrawArrays(mode, 0, vertexBuffer.count);
	};

	drawBuffer(Buffer::LINES, GL_LINES);
	drawBuffer(Buffer::TRIANGLES, GL_TRIANGLES);
	glDisable(GL_DEPTH_TEST);
	drawBuffer(Buffer::NO_DEPTH_TEST_LINES, GL_LINES);
	drawBuffer(Buffer::NO_DEPTH_TEST_TRIANGLES, GL_TRIANGLES);
	glEnable(GL_DEPTH_TEST);

	glBindVertexArray(0);
	shaderProgram->Unbind();

	for (auto& buffer : vertexBuffers)
		coClear(buffer);
}
