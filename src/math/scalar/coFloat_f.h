// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "debug/log/coAssert.h"

const coFloat coFloat_pi = 3.14159265f;
const coFloat coFloat_pi2 = 9.86960440f;
const coFloat coFloat_2Pi = 6.28318531f;
const coFloat coFloat_inv2Pi = 0.159154943092f;
const coFloat coFloat_halfPi = 1.57079633f;
const coFloat coFloat_inf = 0x7f800000;
const coFloat coFloat_qNaN = 0x7fc00000;
const coFloat coFloat_e = 2.71828183f;
const coFloat coFloat_largeValue = 1e18f; // such as coFloat_largeValue² < coFloat_max
const coFloat coFloat_inv255 = static_cast<coFloat>(1. / 255.);

coFORCE_INLINE coFloat coMin(coFloat _a, coFloat _b) { return _a < _b ? _a : _b; }
coFORCE_INLINE coFloat coMin(coFloat _a, coFloat _b, coFloat _c) { return coMin(_a, coMin(_b, _c)); }
coFORCE_INLINE coUint32 coMinIndex(coFloat _a, coFloat _b, coFloat _c) { return _a < _b ? (_a < _c ? 0 : 2) : (_b < _c ? 1 : 2); }
coFORCE_INLINE coFloat coMax(coFloat _a, coFloat _b) { return _a > _b ? _a : _b; }
coFORCE_INLINE coFloat coMax(coFloat _a, coFloat _b, coFloat _c) { return coMax(_a, coMax(_b, _c)); }
coFORCE_INLINE coUint32 coMaxIndex(coFloat _a, coFloat _b, coFloat _c) { return _a > _b ? (_a > _c ? 0 : 2) : (_b > _c ? 1 : 2); }
coFORCE_INLINE coFloat coClamp(coFloat _x, coFloat _min, coFloat _max) { return coMin(_max, coMax(_min, _x)); }
coFORCE_INLINE coFloat coClamp01(coFloat _x) { return coClamp(_x, 0.f, 1.f); }
coFORCE_INLINE coFloat coAbs(coFloat _x) { return ::fabsf(_x); }
coFORCE_INLINE coBool coEqual(coFloat _x, coFloat _y, coFloat _eps = 0.0001f) { return ::fabsf(_x - _y) < _eps; }
coFORCE_INLINE coBool coEqual0(coFloat _x, coFloat _eps = 0.0001f) { return ::fabsf(_x) < _eps; }
coFORCE_INLINE coBool coNotEqual(coFloat _x, coFloat _y, coFloat _eps = 0.0001f) { return ::fabsf(_x - _y) > _eps; }
coFORCE_INLINE coBool coNotEqual0(coFloat _x, coFloat _eps = 0.0001f) { return ::fabsf(_x) > _eps; }
coFORCE_INLINE coFloat coSqrt(coFloat _x) { return ::sqrtf(_x); }
coFORCE_INLINE coFloat coPow(coFloat _x, coFloat _exponent) { return ::powf(_x, _exponent); }
coFORCE_INLINE coFloat coPow2(coFloat _x) { return _x * _x; }
coFORCE_INLINE coFloat coPow3(coFloat _x) { return _x * _x * _x; }
coFORCE_INLINE coFloat coPow4(coFloat _x) { const coFloat x2 = _x * _x; return x2 * x2; }
coFORCE_INLINE coFloat coExp(coFloat _x) { return ::expf(_x); }
coFORCE_INLINE coFloat coExp2(coFloat _x) { return ::powf(2.0f, _x); }
coFORCE_INLINE coFloat coLog10(coFloat _x) { return ::log10f(_x); }
coFORCE_INLINE coFloat coLn(coFloat _x) { return ::logf(_x); }
coFORCE_INLINE coFloat coMod(coFloat _x, coFloat _y) { return ::fmodf(_x, _y); }
coFORCE_INLINE coFloat coCeil(coFloat _x) { return ::ceilf(_x); }
coFORCE_INLINE coBool coIsInRange01(coFloat _x) { return (_x >= 0.f) && (_x <= 1.f); }
coFORCE_INLINE coBool coIsInRange(coFloat _x, coFloat _min, coFloat _max) { return (_x <= _max) && (_x >= _min); }
coFORCE_INLINE coFloat coLerp(coFloat _x, coFloat _y, coFloat _t) { return _x + _t * (_y - _x); }
coFORCE_INLINE coFloat coSign(coFloat _x) { return (_x >= 0.f) ? 1.f : -1.f; }
coFORCE_INLINE coFloat coConvertToRad(coFloat _degrees) { return _degrees * 0.01745329252f; }
coFORCE_INLINE coFloat coConvertToDeg(coFloat _radians) { return _radians * 57.2957795131f; }
coFORCE_INLINE coFloat coCos(coFloat _x) { return ::cosf(_x); }
coFORCE_INLINE coFloat coSin(coFloat _x) { return ::sinf(_x); }
coFORCE_INLINE coFloat coTan(coFloat _x) { return ::tanf(_x); }
coFORCE_INLINE coBool coNearEqual(const coFloat& _a, const coFloat& _b, const coFloat& _epsilon = 0.0001f) { return coAbs(_b - _a) < _epsilon; }
coFORCE_INLINE coBool coNearEqual0(const coFloat& _a, const coFloat& _epsilon = 0.0001f) { return coAbs(_a) < _epsilon; }

coFORCE_INLINE coFloat coFloor(coFloat _x)
{
	//coTODO("the ? x : x-1 stuff does not provide the same behaviour as floorf for exact coFloat values like 1.f");
	// should be faster than ::floorf
	//return static_cast<coFloat>( (_x >= 0.f) ? convertToInt32(_x) : convertToInt32(_x - 1.f) );
	return ::floorf(_x);
}

coFORCE_INLINE coFloat coWrap(coFloat _x, coFloat _min, coFloat _max)
{
	coASSERT(_min < _max);
	const coFloat range = _max - _min;
	return _x - range * coFloor((_x - _min) / range);
}

coFORCE_INLINE coFloat coCopySign(coFloat _x, coFloat _from)
{
#ifdef coMSVC_COMPILER
	return static_cast<coFloat>(_copysign(_x, _from));
#else
	return std::copysign(_x, _from);
#endif
}

/// requires tests
coFORCE_INLINE coFloat coLog2(coFloat _x)
{
	coUint32 c = coBitCast<coUint32>(_x);
	c = (c >> 23) - 127;

	return coBitCast<coFloat>(c);
}

coFORCE_INLINE coBool coIsFinite(coFloat _x)
{
	return (_x + 1) != _x;
}

coFORCE_INLINE coBool coIsValid(coFloat _x)
{
	return _x == _x;
}

coFORCE_INLINE coFloat coAcos(coFloat _x)
{
	coASSERT(coIsInRange(_x, -1.f, 1.f));
	return ::acosf(_x);
}

coFORCE_INLINE coFloat coAsin(coFloat _x)
{
	coASSERT(coIsInRange(_x, -1.f, 1.f));
	return ::asinf(_x);
}

coFORCE_INLINE coFloat coAtan(coFloat _x, coFloat _y)
{
	coASSERT(coAbs(_x) > 0.00001f || coAbs(_y) > 0.00001f);
	return ::atan2f(_x, _y);
}

coFORCE_INLINE coFloat coRandFloat(coUint32& _seed)
{
	coASSERT(_seed);
	_seed *= 16807;
	return (static_cast<coFloat>(_seed)) / static_cast<coFloat>(0x80000000);
}

/// If 0, returns a very small coFloat. Returns the same value instead.
/// [opt] can be used to avoid a branch
coFORCE_INLINE coFloat coDenullify(coFloat _x)
{
	return _x + 1.0e-037f; // very small coFloat
}

coFORCE_INLINE coFloat coRemainder(coFloat _x, coFloat _y)
{
	coASSERT(_y != 0.f);
	return _x - coFloor(_x / _y) * _y;
}
