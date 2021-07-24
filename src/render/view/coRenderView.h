// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/matrix/coMat4.h"
#include "container/array/coArray.h"

class coRenderView
{
public:
	void SetViewport(coInt x, coInt y, coInt width, coInt height);
	void SetView(const coMat4& view_) { view = view_; }
	void SetProjection(const coMat4& proj_) { proj = proj; }

	void Begin();
	void End();

private:
	coInt x = 0;
	coInt y = 0;
	coInt width = 0;
	coInt height = 0;
	coMat4 view;
	coMat4 proj;
};
