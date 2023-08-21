// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"

class alignas(16) coBool32x3
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coBool32x3);
	coDECLARE_SIMD();
public:
	coFORCE_INLINE explicit operator coBool () const { return (_mm_movemask_ps(coBitCast<__m128>(*this)) & 7) == 7; }

	coUint32 x;
	coUint32 y;
	coUint32 z;
private:
	coUint32 pad;
};

/*
class alignas(16) coBool32x3Splat : public coBool32x3
{
public:
	coFORCE_INLINE operator coBool () const { return (_mm_movemask_ps(coBitCast<__m128>(*this)) & 7) == 7; }
};
*/