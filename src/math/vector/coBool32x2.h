// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coTypeDecl.h"

class alignas(16) coBool32x2
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coBool32x2);
public:
	coFORCE_INLINE operator coBool () const { return (_mm_movemask_ps(coBitCast<__m128>(*this)) & 3) == 3; }

	coUint32 x;
	coUint32 y;
private:
	coUint32 pad0;
	coUint32 pad1;
};
