// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"
#include "debug/log/coAssert.h"
#include "math/scalar/coUint32_f.h"

constexpr coFloat coFloat_pi = 3.14159265f;
constexpr coFloat coFloat_pi2 = 9.86960440f;
constexpr coFloat coFloat_2Pi = 6.28318531f;
constexpr coFloat coFloat_inv2Pi = 0.159154943092f;
constexpr coFloat coFloat_halfPi = 1.57079633f;
constexpr coFloat coFloat_inf = 0x7f800000;
constexpr coFloat coFloat_qNaN = 0x7fc00000;
constexpr coFloat coFloat_e = 2.71828183f;
constexpr coFloat coFloat_NearEqualDefaultEpsilon = 1e-4f;
constexpr coFloat coFloat_largeValue = 1e18f; // such as coFloat_largeValue² < coFloat_max
constexpr coFloat coFloat_inv255 = static_cast<coFloat>(1. / 255.);

coFORCE_INLINE constexpr coFloat coMin(coFloat _a, coFloat _b) { return _a < _b ? _a : _b; }
coFORCE_INLINE constexpr coFloat coMin(coFloat _a, coFloat _b, coFloat _c) { return coMin(_a, coMin(_b, _c)); }
coFORCE_INLINE constexpr coUint32 coMinIndex(coFloat _a, coFloat _b, coFloat _c) { return _a < _b ? (_a < _c ? 0 : 2) : (_b < _c ? 1 : 2); }
coFORCE_INLINE constexpr coFloat coMax(coFloat _a, coFloat _b) { return _a > _b ? _a : _b; }
coFORCE_INLINE constexpr coFloat coMax(coFloat _a, coFloat _b, coFloat _c) { return coMax(_a, coMax(_b, _c)); }
coFORCE_INLINE constexpr coUint32 coMaxIndex(coFloat _a, coFloat _b, coFloat _c) { return _a > _b ? (_a > _c ? 0 : 2) : (_b > _c ? 1 : 2); }
coFORCE_INLINE constexpr coFloat coClamp(coFloat _x, coFloat _min, coFloat _max) { return coMin(_max, coMax(_min, _x)); }
coFORCE_INLINE constexpr coFloat coClamp01(coFloat _x) { return coClamp(_x, 0.f, 1.f); }
coFORCE_INLINE coFloat coAbs(coFloat _x) { return std::abs(_x); }
coFORCE_INLINE coBool coNearEqual(coFloat _x, coFloat _y, coFloat _eps = coFloat_NearEqualDefaultEpsilon) { return coAbs(_x - _y) < _eps; }
coFORCE_INLINE coBool coNearEqual0(coFloat _x, coFloat _eps = coFloat_NearEqualDefaultEpsilon) { return coAbs(_x) < _eps; }
coFORCE_INLINE coBool coNotNearEqual(coFloat _x, coFloat _y, coFloat _eps = coFloat_NearEqualDefaultEpsilon) { return coAbs(_x - _y) > _eps; }
coFORCE_INLINE coBool coNotNearEqual0(coFloat _x, coFloat _eps = coFloat_NearEqualDefaultEpsilon) { return coAbs(_x) > _eps; }
coFORCE_INLINE coFloat coSquareRoot(coFloat _x) { return std::sqrt(_x); }
coFORCE_INLINE coFloat coCubicRoot(coFloat _x) { return std::cbrt(_x); }
coFORCE_INLINE coFloat coInvSquareRoot(coFloat _x) { coASSERT(_x > 0.0f); return 1.0f / std::sqrt(_x); }
coFORCE_INLINE coFloat coPow(coFloat _x, coFloat _exponent) { return std::pow(_x, _exponent); }
coFORCE_INLINE constexpr coFloat coPow2(coFloat _x) { return _x * _x; }
coFORCE_INLINE constexpr coFloat coPow3(coFloat _x) { return _x * _x * _x; }
coFORCE_INLINE coFloat coPow4(coFloat _x) { const coFloat x2 = _x * _x; return x2 * x2; }
coFORCE_INLINE coFloat coExp(coFloat _x) { return std::exp(_x); }
coFORCE_INLINE coFloat coExp2(coFloat _x) { return std::exp2(_x); }
coFORCE_INLINE coFloat coExpMinusOne(coFloat _x) { return std::expm1(_x); }
coFORCE_INLINE coFloat coLog10(coFloat _x) { return std::log10(_x); }
coFORCE_INLINE coFloat coLog(coFloat _x) { return std::log(_x); }
coFORCE_INLINE coFloat coMod(coFloat _x, coFloat _y) { return std::fmod(_x, _y); }
coFORCE_INLINE coFloat coCeil(coFloat _x) { return std::ceil(_x); }
coFORCE_INLINE constexpr coBool coIsInRange01(coFloat _x) { return (_x >= 0.f) && (_x <= 1.f); }
coFORCE_INLINE constexpr coBool coIsInRange(coFloat _x, coFloat _min, coFloat _max) { return (_x <= _max) && (_x >= _min); }
coFORCE_INLINE constexpr coFloat coLerp(coFloat _x, coFloat _y, coFloat _t) { return _x + _t * (_y - _x); }
coFORCE_INLINE constexpr coFloat coSign(coFloat _x) { return (_x >= 0.f) ? 1.f : -1.f; }
coFORCE_INLINE constexpr coFloat coConvertToRad(coFloat _degrees) { return _degrees * 0.01745329252f; }
coFORCE_INLINE constexpr coFloat coConvertToDeg(coFloat _radians) { return _radians * 57.2957795131f; }
coFORCE_INLINE coFloat coCos(coFloat _x) { return std::cos(_x); }
coFORCE_INLINE coFloat coSin(coFloat _x) { return std::sin(_x); }
coFORCE_INLINE coFloat coTan(coFloat _x) { return std::tan(_x); }
coFORCE_INLINE coFloat coTanh(coFloat _x) { return std::tanh(_x); }
coFORCE_INLINE coBool coNearEqual(const coFloat& _a, const coFloat& _b, const coFloat& _epsilon = 0.0001f) { return coAbs(_b - _a) < _epsilon; }
coFORCE_INLINE coBool coNearEqual0(const coFloat& _a, const coFloat& _epsilon = 1e-4f) { return coAbs(_a) < _epsilon; }
coFORCE_INLINE coFloat coDistance(const coFloat _a, const coFloat _b) { return std::fdim(_a, _b); }

coFORCE_INLINE coFloat coFloor(coFloat _x)
{
	//coTODO("the ? x : x-1 stuff does not provide the same behaviour as floorf for exact coFloat values like 1.f");
	// should be faster than ::floorf
	//return static_cast<coFloat>( (_x >= 0.f) ? convertToInt32(_x) : convertToInt32(_x - 1.f) );
	return std::floor(_x);
}

coFORCE_INLINE coFloat coWrap(coFloat _x, coFloat _min, coFloat _max)
{
	coASSERT(_min < _max);
	const coFloat range = _max - _min;
	return _x - range * coFloor((_x - _min) / range);
}

coFORCE_INLINE coFloat coCopySign(coFloat _x, coFloat _from)
{
	return std::copysign(_x, _from);
}

/// requires tests
coFORCE_INLINE coFloat coLog2(coFloat _x)
{
	coUint32 c = coBitCast<coUint32>(_x);
	c = (c >> 23) - 127;

	return coBitCast<coFloat>(c);
}

coFORCE_INLINE constexpr coBool coIsFinite(coFloat _x)
{
	return (_x + 1) != _x;
}

coFORCE_INLINE constexpr coBool coIsValid(coFloat _x)
{
	return _x == _x;
}

coFORCE_INLINE coFloat coAcos(coFloat _x)
{
	coASSERT(coIsInRange(_x, -1.f, 1.f));
	return std::acos(_x);
}

coFORCE_INLINE coFloat coAsin(coFloat _x)
{
	coASSERT(coIsInRange(_x, -1.f, 1.f));
	return std::asin(_x);
}

coFORCE_INLINE coFloat coAtan(coFloat _x, coFloat _y)
{
	coASSERT(coAbs(_x) > 0.00001f || coAbs(_y) > 0.00001f);
	return std::atan2(_x, _y);
}

coFORCE_INLINE coFloat coRandFloat(coUint32& _seed)
{
	return coFloat(coRand(_seed)) / coRAND_MAX;
}

coFORCE_INLINE coFloat coRand11(coUint32& _seed)
{
	coASSERT(_seed != 0);
	// http://iquilezles.org/www/articles/sfrand/sfrand.htm
	_seed *= 16807;
	const coFloat r = coBitCast<coFloat>((_seed >> 9) | 0x40000000) - 3.0f;
	coASSERT(coIsInRange(r, -1.0f, 1.0f));
	return r;
}

coFORCE_INLINE coFloat coRand01(coUint32& _seed)
{
	coASSERT(_seed != 0);
	// http://iquilezles.org/www/articles/sfrand/sfrand.htm
	_seed *= 16807;
	return coBitCast<coFloat>((_seed >> 9) | 0x3f800000) - 1.0f;
}

/// If 0, returns a very small coFloat. Returns the same value instead.
/// [opt] can be used to avoid a branch
coFORCE_INLINE constexpr coFloat coDenullify(coFloat _x)
{
	return _x + 1.0e-037f; // very small coFloat
}

coFORCE_INLINE coFloat coRemainder(coFloat _x, coFloat _y)
{
	coASSERT(_y != 0.f);
	return std::remainder(_x, _y);
}
