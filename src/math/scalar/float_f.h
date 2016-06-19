// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/types/baseTypes.h"
#include "lang/cppExtensions.h"
#include "debug/log.h"

namespace co
{
	const float float_pi = 3.14159265f;
	const float float_pi2 = 9.86960440f;
	const float float_2Pi = 6.28318531f;
	const float float_halfPi = 1.57079633f;
	const float float_inf = 0x7f800000;
	const float float_qNaN = 0x7fc00000;
	const float float_e = 2.71828183f;
	const float float_largeValue = 1e18f; // such as float_largeValue² < float_max
	const float float_inv255 = static_cast<float>(1. / 255.);

	coFORCE_INLINE float min(float _a, float _b) { return _a < _b ? _a : _b; }
	coFORCE_INLINE float min(float _a, float _b, float _c) { return min(_a, min(_b, _c)); }
	coFORCE_INLINE uint32 minIndex(float _a, float _b, float _c) { return _a < _b ? (_a < _c ? 0 : 2) : (_b < _c ? 1 : 2); }
	coFORCE_INLINE float max(float _a, float _b) { return _a > _b ? _a : _b; }
	coFORCE_INLINE float max(float _a, float _b, float _c) { return max(_a, max(_b, _c)); }
	coFORCE_INLINE uint32 maxIndex(float _a, float _b, float _c) { return _a > _b ? (_a > _c ? 0 : 2) : (_b > _c ? 1 : 2); }
	coFORCE_INLINE float clamp(float _x, float _min, float _max) { return min(_max, max(_min, _x)); }
	coFORCE_INLINE float clamp01(float _x) { return clamp(_x, 0.f, 1.f); }
	coFORCE_INLINE float abs(float _x) { return ::fabsf(_x); }
	coFORCE_INLINE bool equal(float _x, float _y, float _eps = 0.0001f) { return ::fabsf(_x - _y) < _eps; }
	coFORCE_INLINE bool equal0(float _x, float _eps = 0.0001f) { return ::fabsf(_x) < _eps; }
	coFORCE_INLINE bool notEqual(float _x, float _y, float _eps = 0.0001f) { return ::fabsf(_x - _y) > _eps; }
	coFORCE_INLINE bool notEqual0(float _x, float _eps = 0.0001f) { return ::fabsf(_x) > _eps; }
	coFORCE_INLINE float sqrt(float _x) { return ::sqrtf(_x); }
	coFORCE_INLINE float pow(float _x, float _exponent) { return ::powf(_x, _exponent); }
	coFORCE_INLINE float pow2(float _x) { return _x * _x; }
	coFORCE_INLINE float pow3(float _x) { return _x * _x * _x; }
	coFORCE_INLINE float pow4(float _x) { const float x2 = _x * _x; return x2 * x2; }
	coFORCE_INLINE float exp(float _x) { return ::expf(_x); }
	coFORCE_INLINE float exp2(float _x) { return ::powf(2.0f, _x); }
	coFORCE_INLINE float log10(float _x) { return ::log10f(_x); }
	coFORCE_INLINE float ln(float _x) { return ::logf(_x); }
	coFORCE_INLINE float mod(float _x, float _y) { return ::fmodf(_x, _y); }
	coFORCE_INLINE float ceil(float _x) { return ::ceilf(_x); }
	coFORCE_INLINE bool isInRange01(float _x) { return (_x >= 0.f) && (_x <= 1.f); }
	coFORCE_INLINE bool isInRange(float _x, float _min, float _max) { return (_x <= _max) && (_x >= _min); }
	coFORCE_INLINE float lerp(float _x, float _y, float _t) { return _x + _t * (_y - _x); }
	coFORCE_INLINE float sign(float _x) { return (_x >= 0.f) ? 1.f : -1.f; }
	coFORCE_INLINE float toRad(float _degrees) { return _degrees * 0.01745329252f; }
	coFORCE_INLINE float toDeg(float _radians) { return _radians * 57.2957795131f; }
	coFORCE_INLINE float cos(float _x) { return ::cosf(_x); }
	coFORCE_INLINE float sin(float _x) { return ::sinf(_x); }
	coFORCE_INLINE float tan(float _x) { return ::tanf(_x); }
	coFORCE_INLINE uint32 castAsUint32(float _x) { return bitCast<uint32>(_x); }
	coFORCE_INLINE uint32 castAsInt32(float _x) { return bitCast<int32>(_x); }

	coFORCE_INLINE float floor(float _x)
	{
		//coTODO("the ? x : x-1 stuff does not provide the same behaviour as floorf for exact float values like 1.f");
		// should be faster than ::floorf
		//return static_cast<float>( (_x >= 0.f) ? convertToInt32(_x) : convertToInt32(_x - 1.f) );
		return ::floorf(_x);
	}

	coFORCE_INLINE float wrap(float _x, float _min, float _max)
	{
		coASSERT(_min < _max);
		const float range = _max - _min;
		return _x - range * floor((_x - _min) / range);
	}

	coFORCE_INLINE float copySign(float _x, float _from)
	{
#ifdef coMSVC_COMPILER
		return static_cast<float>(_copysign(_x, _from));
#else
		return std::copysign(_x, _from);
#endif
	}

	/// requires tests
	coFORCE_INLINE float log2(float _x)
	{
		uint32 c = bitCast<uint32>(_x);
		c = (c >> 23) - 127;

		return bitCast<float>(c);
	}

	coFORCE_INLINE bool isFinite(float _x)
	{
		return (_x + 1) != _x;
	}

	coFORCE_INLINE bool isValid(float _x)
	{
		return _x == _x;
	}

	coFORCE_INLINE float acos(float _x)
	{
		coASSERT(isInRange(_x, -1.f, 1.f));
		return ::acosf(_x);
	}

	coFORCE_INLINE float asin(float _x)
	{
		coASSERT(isInRange(_x, -1.f, 1.f));
		return ::asinf(_x);
	}

	coFORCE_INLINE float atan(float _x, float _y)
	{
		coASSERT(abs(_x) > 0.00001f || abs(_y) > 0.00001f);
		return ::atan2f(_x, _y);
	}

	coFORCE_INLINE float randFloat(uint32& _seed)
	{
		coASSERT(_seed);
		_seed *= 16807;
		return (static_cast<float>(_seed)) / static_cast<float>(0x80000000);
	}

	/// If 0, returns a very small float. Returns the same value instead.
	/// [opt] can be used to avoid a branch
	coFORCE_INLINE float denullify(float _x)
	{
		return _x + 1.0e-037f; // very small float
	}

	coFORCE_INLINE float remainder(float _x, float _y)
	{
		coASSERT(_y != 0.f);
		return _x - floor(_x / _y) * _y;
	}
}
