// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class T>
class coQueue
{
	static_assert(std::is_trivially_copyable<T>::value, "Trivially copyable only");
public:
	typedef T ValueType;
	coQueue() = default;
	T* data = nullptr;
	coUint32 begin = 0;
	coUint32 count = 0;
};
