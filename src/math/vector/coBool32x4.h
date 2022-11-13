// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"
#include "math/vector/coBool32x3.h"

class coBool32x3;

class alignas(16) coBool32x4
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coBool32x4);
public:
	coFORCE_INLINE operator coBool () const { return _mm_movemask_ps(coBitCast<__m128>(*this)) == 15; }
	coFORCE_INLINE operator const coBool32x3& () const { return reinterpret_cast<const coBool32x3&>(*this); }
	coFORCE_INLINE coBool32x4() {}
	coFORCE_INLINE coBool32x4(const coBool32x3& _xyz, coUint32 _w) : x(_xyz.x), y(_xyz.y), z(_xyz.z), w(_w) {}
	coFORCE_INLINE coBool32x4(const coBool32x3& _xyz, coBool _w) : x(_xyz.x), y(_xyz.y), z(_xyz.z), w(_w?0xffffffff : 0) {}

	coNO_INIT coUint32 x;
	coNO_INIT coUint32 y;
	coNO_INIT coUint32 z;
	coNO_INIT coUint32 w;
};
