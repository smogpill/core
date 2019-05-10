// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#if !defined(coPIMPL_PARENT_ACCESS)
#define coPIMPL_PARENT_ACCESS private
#endif
#define coDECLARE_PIMPL(_size_, _alignment_)\
	coPIMPL_PARENT_ACCESS:\
	std::aligned_storage<_size_, _alignment_>::type pimpl;\
	private:

#define coACCESS_IMPL(_implType_) _implType_& impl = reinterpret_cast<_implType_&>(pimpl)
#define coCONSTRUCT_IMPL(_implType_)\
	static_assert(sizeof(_implType_) <= sizeof(pimpl), "");\
	static_assert(alignof(_implType_) == alignof(decltype(pimpl)), "");\
	new (&pimpl) _implType_()
#define coDESTRUCT_IMPL(_implType_) reinterpret_cast<_implType_&>(pimpl).~_implType_()
