// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class K>
struct coHashMapHash
{
	coUint32 operator()(const K& v) const;
};

template <>
struct coHashMapHash<coUint64>
{
	coUint32 operator()(const coUint64& v) const;
};
