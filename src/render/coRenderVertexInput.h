// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

enum class coRenderVertexInput
{
	NONE,
	POS3_NORMAL3_TANGENT3_UV2,
	POS2_UV2_COLOR1
};

template <coRenderVertexInput T>
class coRenderVertex
{
};

template <>
class coRenderVertex<coRenderVertexInput::POS3_NORMAL3_TANGENT3_UV2>
{
public:
	coFloat px, py, pz;
	coFloat nx, ny, nz;
	coFloat tx, ty, tz;
	coFloat u, v;
};

template <>
class coRenderVertex<coRenderVertexInput::POS2_UV2_COLOR1>
{
public:
	coFloat px, py;
	coFloat u, v;
	coUint32 color;
};

coUint coGetNbAttributes(coRenderVertexInput _input);
coUint coGetSize8(coRenderVertexInput _input);
