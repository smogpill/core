// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "coDebugRenderer.h"
#include "lang/result/coResult_f.h"
#include <math/shape/coAabb_f.h>
#include <math/vector/coVec3_f.h>
#include <math/vector/coVec4_f.h>
#include <math/matrix/coMat4_f.h>
#include <ecs/component/coComponent_f.h>
#include <ecs/lib/node/coNode.h>
#include <pattern/pointer/coUniquePtr.h>
#include <container/array/coDynamicArray_f.h>
#include "../mesh/coRenderMesh.h"
#include "../shader/coShader.h"
#include "coDebugRender.h"

coDEFINE_SINGLETON(coDebugRenderer);
coBEGIN_COMPONENT(coDebugRenderer);
coDEFINE_COMPONENT_INIT();
coDEFINE_COMPONENT_SHUTDOWN();
coEND_COMPONENT();

void coDebugRenderer::Init(coEntity& entity)
{
	Base::Init(entity);
	coDebugRenderer::SetInstance(this);
	coTRY_NO_RESULT(InitShaders(), nullptr);
	glCreateVertexArrays(1, &vertexArrayObject);
	glCreateBuffers(1, &vertexBufferObject);
	glCreateBuffers(1, &elementBufferObject);
}

void coDebugRenderer::Shutdown(coEntity& entity)
{
	glDeleteBuffers(1, &vertexBufferObject);
	glDeleteBuffers(1, &elementBufferObject);
	glDeleteVertexArrays(1, &vertexArrayObject);
	delete shader;
	shader = nullptr;
	coDebugRenderer::SetInstance(nullptr);
	Base::Shutdown(entity);
}

coResult coDebugRenderer::InitShaders()
{
	coUniquePtr<coShader> debugProgram = new coShader();
	coTRY(debugProgram->Init("shaders/render/Debug", coShader::VERTEX | coShader::FRAGMENT), nullptr);
	shader = debugProgram.Release();
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
	const coVec3 c(aabb.min + sy);
	const coVec3 d(aabb.min + sx + sy);
	const coVec3 e(a + sz);
	const coVec3 f(b + sz);
	const coVec3 g(c + sz);
	const coVec3 h(d + sz);

	if (options & WIREFRAME)
	{
		DrawLine(a, b, color, options);
		DrawLine(b, d, color, options);
		DrawLine(d, c, color, options);
		DrawLine(c, a, color, options);

		DrawLine(e, f, color, options);
		DrawLine(f, h, color, options);
		DrawLine(h, g, color, options);
		DrawLine(g, e, color, options);

		DrawLine(a, e, color, options);
		DrawLine(b, f, color, options);
		DrawLine(d, h, color, options);
		DrawLine(c, g, color, options);
	}
	else
	{
		DrawTriangle(a, f, b, color, options);
		DrawTriangle(a, e, f, color, options);

		DrawTriangle(b, h, d, color, options);
		DrawTriangle(b, f, h, color, options);

		DrawTriangle(d, g, c, color, options);
		DrawTriangle(d, h, g, color, options);

		DrawTriangle(c, e, a, color, options);
		DrawTriangle(c, g, e, color, options);

		// Top
		DrawTriangle(b, c, a, color, options);
		DrawTriangle(b, d, c, color, options);
		
		// Bottom
		DrawTriangle(e, h, f, color, options);
		DrawTriangle(e, g, h, color, options);
	}
}

void coDebugRenderer::DrawBox(const coMat4& mat, const coColor& color, coUint32 options)
{
	const coVec3 pos = mat.c3;
	const coVec3 vx = mat.c0;
	const coVec3 vy = mat.c1;
	const coVec3 vz = mat.c2;

	const coVec3 a(pos - vx - vy - vz);
	const coVec3 b(pos + vx - vy - vz);
	const coVec3 c(pos - vx + vy - vz);
	const coVec3 d(pos + vx + vy - vz);
	const coVec3 e(pos - vx - vy + vz);
	const coVec3 f(pos + vx - vy + vz);
	const coVec3 g(pos - vx + vy + vz);
	const coVec3 h(pos + vx + vy + vz);

	if (options & WIREFRAME)
	{
		DrawLine(a, b, color, options);
		DrawLine(b, d, color, options);
		DrawLine(d, c, color, options);
		DrawLine(c, a, color, options);

		DrawLine(e, f, color, options);
		DrawLine(f, h, color, options);
		DrawLine(h, g, color, options);
		DrawLine(g, e, color, options);

		DrawLine(a, e, color, options);
		DrawLine(b, f, color, options);
		DrawLine(d, h, color, options);
		DrawLine(c, g, color, options);
	}
	else
	{
		DrawTriangle(a, f, b, color, options);
		DrawTriangle(a, e, f, color, options);

		DrawTriangle(b, h, d, color, options);
		DrawTriangle(b, f, h, color, options);

		DrawTriangle(d, g, c, color, options);
		DrawTriangle(d, h, g, color, options);

		DrawTriangle(c, e, a, color, options);
		DrawTriangle(c, g, e, color, options);

		// Top
		DrawTriangle(b, c, a, color, options);
		DrawTriangle(b, d, c, color, options);

		// Bottom
		DrawTriangle(e, h, f, color, options);
		DrawTriangle(e, g, h, color, options);
	}
}

void coDebugRenderer::DrawFrame(const coMat4& frame, coFloat size, coUint32 options)
{
	DrawLine(coVec3(frame.c3), coVec3(frame.c3 + frame.c0 * size), coColor::s_coolRed, options);
	DrawLine(coVec3(frame.c3), coVec3(frame.c3 + frame.c1 * size), coColor::s_coolGreen, options);
	DrawLine(coVec3(frame.c3), coVec3(frame.c3 + frame.c2 * size), coColor::s_coolBlue, options);
}

void coDebugRenderer::DrawDot(const coVec3& pos, coFloat radius, const coColor& color, coUint32 options)
{
	Draw(coAabb(-coVec3(radius), coVec3(radius)) + pos, color, options);
}

void coDebugRenderer::Render(const coMat4& viewProj)
{
	shader->Bind();

	const auto modelViewProjLocation = shader->GetUniformLocation("modelViewProj");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	// Draw renderers
	{
		for (const coDebugRender* renderer : renderers)
		{
			const coRenderMesh* renderMesh = renderer->GetRenderMesh();
			if (renderMesh)
			{
				const coNode* node = renderer->GetNode();
				coASSERT(node);
				const coMat4 model = coMat4(node->GetGlobal());
				shader->SetUniform(modelViewProjLocation, model * viewProj);
				renderMesh->Draw();
			}
		}
	}

	// Draw internal buffers
	{
		shader->SetUniform(modelViewProjLocation, viewProj);
		glBindVertexArray(vertexArrayObject);

		auto drawBuffer = [&](Buffer buffer, GLenum mode)
		{
			auto& vertexBuffer = vertexBuffers[buffer];
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
			glBufferData(GL_ARRAY_BUFFER, vertexBuffer.count * sizeof(Vertex), vertexBuffer.data, GL_STREAM_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
			glDrawArrays(mode, 0, vertexBuffer.count);
		};

		drawBuffer(Buffer::LINES, GL_LINES);
		drawBuffer(Buffer::TRIANGLES, GL_TRIANGLES);
		glDisable(GL_DEPTH_TEST);
		drawBuffer(Buffer::NO_DEPTH_TEST_LINES, GL_LINES);
		drawBuffer(Buffer::NO_DEPTH_TEST_TRIANGLES, GL_TRIANGLES);
		glEnable(GL_DEPTH_TEST);

		glBindVertexArray(0);
	}
	
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	shader->Unbind();

	for (auto& buffer : vertexBuffers)
		coClear(buffer);
}

void coDebugRenderer::Add(coDebugRender& render)
{
	coASSERT(!coContains(renderers, &render));
	coPushBack(renderers, &render);
}

void coDebugRenderer::Remove(coDebugRender& render)
{
	coRemoveUnordered(renderers, &render);
}