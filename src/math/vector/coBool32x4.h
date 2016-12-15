// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"

class alignas(16) coBool32x4
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE operator coBool () const { return (_mm_movemask_ps(coBitCast<__m128>(*this)) & 15) == 15; }
	coFORCE_INLINE operator const coBool32x3& () const { return reinterpret_cast<const coBool32x3&>(*this); }

	coUint32 x;
	coUint32 y;
	coUint32 z;
	coUint32 w;
};
