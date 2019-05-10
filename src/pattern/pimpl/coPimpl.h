// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#define coDECLARE_PIMPL(_size_, _alignment_)\
	std::aligned_storage<_size_, _alignment_>::type pimpl;\
	template <class T> T& GetImpl() \
	{ \
		static_assert(sizeof(T) <= sizeof(pimpl), ""); \
		static_assert(alignof(T) == alignof(decltype(pimpl)), ""); \
		return reinterpret_cast<T&>(pimpl);\
	}
