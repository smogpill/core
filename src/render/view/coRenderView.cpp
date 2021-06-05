// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/view/coRenderView.h"
#include "math/vector/coVec2.h"
#include "math/vector/coVec3.h"

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
