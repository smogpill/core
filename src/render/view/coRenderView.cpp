// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/view/coRenderView.h"
#include "math/vector/coVec2.h"
#include "math/vector/coVec3.h"
#include "container/array/coArray_f.h"

void coRenderView::SetViewport(coInt x_, coInt y_, coUint width_, coUint height_)
{
	x = x_;
	y = y_;
	width = width_;
	height = height_;
}

void coRenderView::Begin()
{
	glViewport(x, y, width, height);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	/*
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(&view.c0.x);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(&proj.c0.x);
	*/
}

void coRenderView::End()
{
}
