// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include <math/pch.h>
#include <math/hash/coHash_f.h>
#include <math/scalar/coUint32_f.h>
#include <math/scalar/coUint64_f.h>

#ifdef coMSVC_COMPILER
#define coBIG_CONSTANT(_n_) (_n_)
#else
#define coBIG_CONSTANT(_n_) (_n_##LLU)
#endif

coFORCE_INLINE static coUint32 coFmix32(coUint32 _h)
{
	_h ^= _h >> 16;
	_h *= 0x85ebca6b;
	_h ^= _h >> 13;
	_h *= 0xc2b2ae35;
	_h ^= _h >> 16;

	return _h;
}

coFORCE_INLINE static coUint64 coFmix64(coUint64 _k)
{
	_k ^= _k >> 33;
	_k *= coBIG_CONSTANT(0xff51afd7ed558ccd);
	_k ^= _k >> 33;
	_k *= coBIG_CONSTANT(0xc4ceb9fe1a85ec53);
	_k ^= _k >> 33;

	return _k;
}

static void coMurmurHash3_x86_32(coUint32* _out, const void* _data, coInt _len)
{
	const coByte* data = static_cast<const coByte*>(_data);
	const coInt nblocks = _len / 4;

	coUint32 h1 = 0;

	const coUint32 c1 = 0xcc9e2d51;
	const coUint32 c2 = 0x1b873593;

	// Body
	const coUint32* blocks = reinterpret_cast<const coUint32*>(data + nblocks * 4);

	for (int i = -nblocks; i; ++i)
	{
		coUint32 k1 = blocks[i];

		k1 *= c1;
		k1 = coRotl(k1, 15);
		k1 *= c2;

		h1 ^= k1;
		h1 = coRotl(h1, 13);
		h1 = h1 * 5 + 0xe6546b64;
	}

	// Tail
	const coUint8* tail = reinterpret_cast<const coUint8*>(data + nblocks * 4);

	coUint32 k1 = 0;

	switch (_len & 3)
	{
	case 3: k1 ^= tail[2] << 16;
	case 2: k1 ^= tail[1] << 8;
	case 1: k1 ^= tail[0];
		k1 *= c1; k1 = coRotl(k1, 15); k1 *= c2; h1 ^= k1;
	};

	// Finalization
	h1 ^= _len;

	h1 = coFmix32(h1);

	*_out = h1;
}

static void coMurmurHash3_x64_128(void* _out, const void* _data, const coInt _len)
{
	const coByte* data = static_cast<const coByte*>(_data);
	const coInt nblocks = _len / 16;

	coUint64 h1 = 0;
	coUint64 h2 = 0;

	const coUint64 c1 = coBIG_CONSTANT(0x87c37b91114253d5);
	const coUint64 c2 = coBIG_CONSTANT(0x4cf5ad432745937f);

	// Body
	const coUint64* blocks = reinterpret_cast<const coUint64*>(data);

	for (int i = 0; i < nblocks; ++i)
	{
		coUint64 k1 = blocks[i * 2 + 0];
		coUint64 k2 = blocks[i * 2 + 1];

		k1 *= c1; k1 = coRotl(k1, 31); k1 *= c2; h1 ^= k1;

		h1 = coRotl(h1, 27); h1 += h2; h1 = h1 * 5 + 0x52dce729;

		k2 *= c2; k2 = coRotl(k2, 33); k2 *= c1; h2 ^= k2;

		h2 = coRotl(h2, 31); h2 += h1; h2 = h2 * 5 + 0x38495ab5;
	}

	// Tail
	const coUint8* tail = reinterpret_cast<const coUint8*>(data + nblocks * 16);

	coUint64 k1 = 0;
	coUint64 k2 = 0;

	switch (_len & 15)
	{
	case 15: k2 ^= ((coUint64)tail[14]) << 48;
	case 14: k2 ^= ((coUint64)tail[13]) << 40;
	case 13: k2 ^= ((coUint64)tail[12]) << 32;
	case 12: k2 ^= ((coUint64)tail[11]) << 24;
	case 11: k2 ^= ((coUint64)tail[10]) << 16;
	case 10: k2 ^= ((coUint64)tail[9]) << 8;
	case  9: k2 ^= ((coUint64)tail[8]) << 0;
		k2 *= c2; k2 = coRotl(k2, 33); k2 *= c1; h2 ^= k2;

	case  8: k1 ^= ((coUint64)tail[7]) << 56;
	case  7: k1 ^= ((coUint64)tail[6]) << 48;
	case  6: k1 ^= ((coUint64)tail[5]) << 40;
	case  5: k1 ^= ((coUint64)tail[4]) << 32;
	case  4: k1 ^= ((coUint64)tail[3]) << 24;
	case  3: k1 ^= ((coUint64)tail[2]) << 16;
	case  2: k1 ^= ((coUint64)tail[1]) << 8;
	case  1: k1 ^= ((coUint64)tail[0]) << 0;
		k1 *= c1; k1 = coRotl(k1, 31); k1 *= c2; h1 ^= k1;
	};

	// Finalization
	h1 ^= _len; h2 ^= _len;

	h1 += h2;
	h2 += h1;

	h1 = coFmix64(h1);
	h2 = coFmix64(h2);

	h1 += h2;
	h2 += h1;

	reinterpret_cast<coUint64*>(_out)[0] = h1;
	reinterpret_cast<coUint64*>(_out)[1] = h2;
}

coUint32 coHash32(coUint32 _val)
{
	return coFmix32(_val);
}

coUint32 coHash32(coUint64 _val)
{
	return reinterpret_cast<coUint32*>(coFmix64(_val))[0];
}

coUint32 coHash32(const void* _data, coUint _len)
{
	if (_len)
	{
		coASSERT(_data);
		coUint32 h;
		coMurmurHash3_x86_32(&h, _data, _len);
		return h;
	}
	else
	{
		return 0;
	}
}

coUint64 coHash64(const void* _data, coUint _len)
{
	if (_len)
	{
		coASSERT(_data);
		coUint64 h[2];
		coMurmurHash3_x64_128(h, _data, _len);
		return h[0];
	}
	else
	{
		return 0;
	}
}

coUint64 coHash64(coUint32 _val)
{
	return coFmix64(_val);
}

coUint64 coHash64(coUint64 _val)
{
	return coFmix64(_val);
}
