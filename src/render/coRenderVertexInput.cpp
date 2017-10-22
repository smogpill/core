// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "render/pch.h"
#include "render/coRenderVertexInput.h"
#include "debug/log/coLog.h"
#include "lang/reflect/coField_f.h"

coUint coGetNbAttributes(coRenderVertexInput _input)
{
	switch (_input)
	{
	case coRenderVertexInput::POS3_NORMAL3_TANGENT3_UV2: return 4;
	case coRenderVertexInput::POS2_UV2_COLOR1: return 3;
	default: 
	{
		coWARN_NOT_AVAILABLE();
		return 0;
	}
	}
}

coUint coGetSize8(coRenderVertexInput _input)
{
	switch (_input)
	{
	case coRenderVertexInput::POS3_NORMAL3_TANGENT3_UV2: return sizeof(coRenderVertex<coRenderVertexInput::POS3_NORMAL3_TANGENT3_UV2>);
	case coRenderVertexInput::POS2_UV2_COLOR1: return sizeof(coRenderVertex<coRenderVertexInput::POS2_UV2_COLOR1>);
	default:
	{
		coWARN_NOT_AVAILABLE();
		return 0;
	}
	}
}
