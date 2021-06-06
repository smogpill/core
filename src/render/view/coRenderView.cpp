// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/view/coRenderView.h"
#include "math/vector/coVec2.h"
#include "math/vector/coVec3.h"
#include "container/array/coArray_f.h"

void coRenderView::SetViewport(coInt x_, coInt y_, coInt width_, coInt height_)
{
	x = x_;
	y = y_;
	width = width_;
	height = height_;
}

void coRenderView::Begin()
{
	glViewport(x, y, width, height);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&view.c0.x);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&proj.c0.x);
}

void coRenderView::End()
{
}

void coRenderView::DrawLine(const coVec3& a, const coVec3& b)
{
	glBegin(GL_LINES);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glEnd();
}

void coRenderView::Draw2DLine(const coVec2& a, const coVec2& b)
{
	glBegin(GL_LINES);
	glVertex2f(a.x, a.y);
	glVertex2f(b.x, b.y);
	glEnd();
}

void coRenderView::DrawTriangle(const coVec3& a, const coVec3& b, const coVec3& c)
{
	glBegin(GL_TRIANGLES);
	glVertex3f(a.x, a.y, a.z);
	glVertex3f(b.x, b.y, b.z);
	glVertex3f(c.x, c.y, c.z);
	glEnd();
}

void coRenderView::DrawTriangles(const coArray<coVec3>& triangles)
{
	coASSERT(triangles.count % 3 == 0);
	glBegin(GL_TRIANGLES);
	for (const coVec3& v : triangles)
	{
		glVertex3f(v.x, v.y, v.z);
	}
	glEnd();
}
