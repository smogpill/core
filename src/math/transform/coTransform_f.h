// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "math/transform/coTransform.h"
#include "math/vector/coBool32x4_f.h"
#include "math/vector/coFloatx3_f.h"
#include "math/quaternion/coQuat_f.h"

coBool32x4 coIsValid(const coTransform& _this);
coFORCE_INLINE coFloatx3 coTransformPosition(const coTransform& _this, const coFloatx3& _pos)
{
	return coRotateVector(_this.rotation, _this.scale * _pos) + _this.translation;
}
coFORCE_INLINE coFloatx3 coTransformPositionNoScale(const coTransform& _this, const coFloatx3& _pos)
{
	return coRotateVector(_this.rotation, _pos) + _this.translation;
}
coFORCE_INLINE coFloatx3 coTransformVector(const coTransform& _this, const coFloatx3& _vec)
{
	return coRotateVector(_this.rotation, _this.scale * _vec);
}
coFORCE_INLINE coFloatx3 coTransformVectorNoScale(const coTransform& _this, const coFloatx3& _vec)
{
	return coRotateVector(_this.rotation, _vec);
}
coFORCE_INLINE coFloatx3 coInverseTransformPosition(const coTransform& _this, const coFloatx3& _pos)
{
	return coInverseRotateVector(_this.rotation, _pos - _this.translation) / _this.scale;
}
coFORCE_INLINE coFloatx3 coInverseTransformPositionNoScale(const coTransform& _this, const coFloatx3& _pos)
{
	return coInverseRotateVector(_this.rotation, _pos - _this.translation);
}
coFORCE_INLINE coFloatx3 coInverseTransformVector(const coTransform& _this, const coFloatx3& _vec)
{
	return coInverseRotateVector(_this.rotation, _vec) / _this.scale;
}
coFORCE_INLINE coFloatx3 coInverseTransformVectorNoScale(const coTransform& _this, const coFloatx3& _vec)
{
	return coInverseRotateVector(_this.rotation, _vec);
}
coFORCE_INLINE coTransform coInverse(const coTransform& _this)
{
	coTransform inv(nullptr);
	inv.scale = 1.f / _this.scale;
	inv.rotation = coConjugate(_this.rotation);
	inv.translation = -coRotateVector(inv.rotation, _this.translation * inv.scale);
	return inv;
}
coFORCE_INLINE coTransform operator * (const coTransform& _a, const coTransform& _b)
{
	coTransform r(nullptr);
	r.rotation = _b.rotation * _a.rotation;
	r.scale = _a.scale * _b.scale;
	r.translation = coRotateVector(_b.rotation, _a.translation * _b.scale) + _b.translation;
	return r;
}
coFORCE_INLINE coTransform coNormalize(const coTransform& _this)
{
	coTransform r(_this);
	r.rotation = coNormalize(r.rotation);
	return r;
}
