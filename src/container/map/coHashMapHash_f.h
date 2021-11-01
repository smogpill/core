// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coHashMapHash.h"
#include <math/hash/coHash_f.h>

template <class K>
coFORCE_INLINE coUint32 coHashMapHash<K>::operator()(const K& v) const
{
	return coHash32(v);
}

coFORCE_INLINE coUint32 coHashMapHash<coUint64>::operator()(const coUint64& v) const
{
	return coFastHash32(v);
}
