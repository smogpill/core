// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coTypeDecl.h"
#include "math/vector/coInt32x3.h"
class coFloatx4;
class coBinaryOutputStream;
class coBinaryInputStream;

class alignas(16) coFloatx3
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coFloatx3()
	{
#ifdef coDEBUG
		coBitCast<__m128>(*this) = _mm_set_ps1(std::numeric_limits<float>::quiet_NaN());
#endif
	}
	coFORCE_INLINE coFloatx3(coFloat _xyz) { coBitCast<__m128>(*this) = _mm_set_ps1(_xyz); }
	coFORCE_INLINE coFloatx3(coFloat _x, coFloat _y, coFloat _z) { coBitCast<__m128>(*this) = _mm_set_ps(_z, _z, _y, _x); }
	coFORCE_INLINE coFloatx3(coFloatx3 _xxx, coFloatx3 _yyy, coFloatx3 _zzz)
	{
		const __m128 xxx = coBitCast<__m128>(_xxx);
		const __m128 yyy = coBitCast<__m128>(_yyy);
		const __m128 zzz = coBitCast<__m128>(_zzz);
		*this = coBitCast<coFloatx3>(_mm_unpacklo_ps(_mm_unpacklo_ps(xxx, zzz), _mm_unpacklo_ps(yyy, yyy)));
	}
	coFORCE_INLINE coFloatx3(const coInt32x3& _xyz)
		: x(coFloat(_xyz.x))
		, y(coFloat(_xyz.y))
		, z(coFloat(_xyz.z))
	{
	}
	coFORCE_INLINE coFloat& operator[](coUint _i) { return (&x)[_i]; }
	coFORCE_INLINE coFloat operator[](coUint _i) const { return (&x)[_i]; }
	//coFORCE_INLINE operator coFloat () const { return x; }
	void Write(coBinaryOutputStream& stream) const;
	void Read(coBinaryInputStream& stream);

	coFloat x;
	coFloat y;
	coFloat z;
protected:
	coFloat pad;
};

