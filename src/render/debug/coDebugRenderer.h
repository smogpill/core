// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <math/vector/coColor.h>
#include <math/vector/coVec3.h>
#include <math/vector/coVec4.h>
#include <container/array/coDynamicArray.h>
#include <lang/result/coResult.h>
#include <pattern/singleton/coSingleton.h>
class coAabb;
class coShaderProgram;
class coShader;
class coMat4;

class coDebugRenderer
{
	coDECLARE_SINGLETON(coDebugRenderer);
public:
	coResult Init();
	void DrawLine(const coVec3& a, const coVec3& b, const coColor& color = coColor::s_white);
	void DrawTriangle(const coVec3& a, const coVec3& b, const coVec3& c, const coColor& color = coColor::s_white);
	void Draw(const coAabb& aabb, const coColor& color = coColor::s_white);
	void DrawWireframe(const coAabb& aabb, const coColor& color = coColor::s_white);

	void Render(const coMat4& viewProj);

private:
	struct Vertex
	{
		Vertex() {}
		Vertex(const coVec3& pos, const coColor& color) : pos(pos), color(color) {}
		coVec3 pos;
		coColor color;
	};
	coResult InitShaders();

	coDynamicArray<Vertex> lines;
	coDynamicArray<Vertex> triangles;
	coDynamicArray<coShader*> shaders;
	coShaderProgram* shaderProgram = nullptr;
	GLuint vertexArrayObject = 0;
	GLuint vertexBufferObject = 0;
	GLuint elementBufferObject = 0;
};
