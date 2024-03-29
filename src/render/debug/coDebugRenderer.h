// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/vector/coColor.h>
#include <math/vector/coVec3.h>
#include <math/vector/coVec4.h>
#include <container/array/coDynamicArray.h>
#include <lang/result/coResult.h>
#include <pattern/singleton/coSingleton.h>
#include <ecs/component/coComponent.h>
#include <ecs/entity/coEntityHandle.h>
class coAabb;
class coShader;
class coMat4;
class coRenderMesh;
class coDebugRender;

class coDebugRenderer : public coComponent
{
	coDECLARE_COMPONENT(coDebugRenderer, coComponent);
	coDECLARE_SINGLETON(coDebugRenderer);
public:
	enum Options
	{
		WIREFRAME = 1 << 0,
		NO_DEPTH_TEST = 1 << 1, // Keep NO_DEPTH_TEST at 2, we use this as a trick to select buffers when drawing.
	};
	void DrawLine(const coVec3& a, const coVec3& b, const coColor& color = coColor::s_white, coUint32 options = 0);
	void DrawTriangle(const coVec3& a, const coVec3& b, const coVec3& c, const coColor& color = coColor::s_white, coUint32 options = 0);
	void Draw(const coAabb& aabb, const coColor& color = coColor::s_white, coUint32 options = 0);
	void DrawBox(const coMat4& mat, const coColor& color = coColor::s_white, coUint32 options = 0);
	void DrawFrame(const coMat4& frame, coFloat size, coUint32 options = 0);
	void DrawDot(const coVec3& pos, coFloat radius, const coColor& color = coColor::s_white, coUint32 options = 0);
	void Add(coDebugRender&);
	void Remove(coDebugRender&);

	void Render(const coMat4& viewProj);
	coShader* GetShader() const { return shader; }

private:
	enum Buffer
	{
		LINES,
		TRIANGLES,
		NO_DEPTH_TEST_LINES = 2,
		NO_DEPTH_TEST_TRIANGLES = 3,

		END
	};
	struct Vertex
	{
		Vertex() {}
		Vertex(const coVec3& pos, const coColor& color) : pos(pos), color(color) {}
		coVec3 pos;
		coColor color;
	};
	coResult InitShaders();
	void Init(coEntity& entity);
	void Shutdown(coEntity& entity);

	coDynamicArray<Vertex> vertexBuffers[Buffer::END];
	coDynamicArray<coDebugRender*> renderers;
	coShader* shader = nullptr;
	GLuint vertexArrayObject = 0;
	GLuint vertexBufferObject = 0;
	GLuint elementBufferObject = 0;
};
