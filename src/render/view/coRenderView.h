// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/matrix/coMat4.h"
#include "math/vector/coUint32x2.h"
#include "container/array/coArray.h"

class coRenderView
{
public:
	void SetViewport(coInt x, coInt y, coUint width, coUint height);
	void SetView(const coMat4& view_) { view = view_; }
	void SetProjection(const coMat4& proj_) { proj = proj; }
	coUint GetWidth() const { return width; }
	coUint GetHeight() const { return height; }
	coUint32x2 GetSize() const { return coUint32x2(width, height); }

	void Begin();
	void End();

private:
	coInt x = 0;
	coInt y = 0;
	coUint width = 0;
	coUint height = 0;
	coMat4 view;
	coMat4 proj;
};
