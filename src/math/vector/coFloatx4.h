// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/reflect/coTypeDecl.h"
#include "math/vector/coFloatx2.h"
#include "math/vector/coFloatx3.h"
class coBinaryOutputStream;
class coBinaryInputStream;

class alignas(16) coFloatx4
{
	coDECLARE_REFLECTED_NO_VIRTUAL();
public:
	coFORCE_INLINE coFloatx4()
	{
#ifdef coDEBUG
		coBitCast<__m128>(*this) = _mm_set_ps1(std::numeric_limits<float>::quiet_NaN());
#endif
	}
	coFORCE_INLINE coFloatx4(coFloat _xyzw) { coBitCast<__m128>(*this) = _mm_set_ps1(_xyzw); }
	coFORCE_INLINE coFloatx4(coFloat _x, coFloat _y, coFloat _z, coFloat _w) { coBitCast<__m128>(*this) = _mm_set_ps(_w, _z, _y, _x); }
	coFORCE_INLINE coFloatx4(coFloatx4 _xxxx, coFloatx4 _yyyy, coFloatx4 _zzzz, coFloatx4 _wwww)
	{
		const __m128 xxxx = coBitCast<__m128>(_xxxx);
		const __m128 yyyy = coBitCast<__m128>(_yyyy);
		const __m128 zzzz = coBitCast<__m128>(_zzzz);
		const __m128 wwww = coBitCast<__m128>(_wwww);
		*this = coBitCast<coFloatx4>(_mm_unpacklo_ps(_mm_unpacklo_ps(xxxx, zzzz), _mm_unpacklo_ps(yyyy, wwww)));
	}
	explicit coFORCE_INLINE coFloatx4(const coFloatx3& _xyz) : x(_xyz.x), y(_xyz.y), z(_xyz.z)
	{
#ifdef coDEBUG
		w = std::numeric_limits<float>::quiet_NaN();
#endif
	}
	coFORCE_INLINE coFloatx4(const coFloatx3& _xyz, const coFloatx4& _wwww) : x(_xyz.x), y(_xyz.y), z(_xyz.z), w(_wwww.w) {}

	coFORCE_INLINE operator const coFloatx2& () const { return reinterpret_cast<const coFloatx2&>(*this); }
	coFORCE_INLINE operator const coFloatx3& () const { return reinterpret_cast<const coFloatx3&>(*this); }
	//coFORCE_INLINE operator coFloat () const { return x; }
	coFORCE_INLINE coFloat& operator[](coUint _i) { return (&x)[_i]; }
	coFORCE_INLINE coFloat operator[](coUint _i) const { return (&x)[_i]; }
	void Write(coBinaryOutputStream& stream) const;
	void Read(coBinaryInputStream& stream);

	coFloat x;
	coFloat y;
	coFloat z;
	coFloat w;
};
