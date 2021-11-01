// Copyright(c) 2016-2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coTypeDecl.h"
class coBinaryOutputStream;
class coBinaryInputStream;

class alignas(16) coFloatx2
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coFloatx2()
	{
#ifdef coDEBUG
		coBitCast<__m128>(*this) = _mm_set_ps1(std::numeric_limits<float>::quiet_NaN());
#endif
	}
	coFORCE_INLINE coFloatx2(coFloat _xy) : x(_xy), y(_xy) {}
	coFORCE_INLINE coFloatx2(coFloat _x, coFloat _y) : x(_x), y(_y) {}
	coFORCE_INLINE coFloatx2(coFloatx2 _xx, coFloatx2 _yy)
	{
		const __m128 xx = coBitCast<__m128>(_xx);
		const __m128 yy = coBitCast<__m128>(_yy);
		*this = coBitCast<coFloatx2>(_mm_unpacklo_ps(xx, yy));
	}
	void Write(coBinaryOutputStream& stream) const;
	void Read(coBinaryInputStream& stream);

	coFloat x;
	coFloat y;
private:
	coFloat pad0;
	coFloat pad1;
};
