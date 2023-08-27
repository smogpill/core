// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/reflect/coTypeDecl.h"
#include "math/vector/coFloatx3.h"
#include "math/vector/coFloatx4.h"
class coBinaryOutputStream;
class coBinaryInputStream;

class coVec3 : public coFloatx3
{
	coDECLARE_CLASS_NO_POLYMORPHISM(coVec3);
	coDECLARE_SIMD();
public:
	using coFloatx3::coFloatx3;
	coFORCE_INLINE coVec3() { coBitCast<__m128>(*this) = _mm_setzero_ps(); }
	coFORCE_INLINE coVec3(const coFloatx3& _xyz) : coFloatx3(_xyz) {}
	coFORCE_INLINE explicit coVec3(const coInt32x3& xyz) : coFloatx3(coFloat(xyz.x), coFloat(xyz.y), coFloat(xyz.z)) {}
	coFORCE_INLINE coVec3(const coFloatx4& _xyz) : coFloatx3(_xyz) {}
	coFORCE_INLINE coVec3(const coFloatx3& _xxx, const coFloatx3& _yyy, const coFloatx3& _zzz) : coFloatx3(_xxx, _yyy, _zzz) {}
	coFORCE_INLINE coVec3(const coFloatx2& _xy, const coFloatx3& _zzz) : coFloatx3(_xy, _zzz) {}
	coFORCE_INLINE explicit coVec3(coNullPtr) {}
	void Write(coBinaryOutputStream& stream) const;
	void Read(coBinaryInputStream& stream);
};
