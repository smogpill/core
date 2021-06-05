// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "math/matrix/coMat4.h"

class coVec3;
class coVec2;

class coRenderView
{
public:
	void DrawLine(const coVec3& a, const coVec3& b);
	void Draw2DLine(const coVec2& a, const coVec2& b);

private:
	coMat4 view;
};
