// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coFloatx2.h"

class coRenderVertex_PosNormalTangentUv
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFloatx3 pos;
	coFloatx3 normal;
	coFloatx3 tangent;
	coFloatx2 uv;
};
