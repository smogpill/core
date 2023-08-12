// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "debug/log/coAssert.h"

#define coDECLARE_SINGLETON(_Class_) \
	public: \
	inline static _Class_* instance = nullptr; \
	template< class... Args > \
	static void CreateInstance(Args&&... args) { coASSERT(instance == nullptr); instance = new _Class_(std::forward<Args>(args)...); } \
	static void CreateInstanceIfMissing() { if (!instance) instance = new _Class_(); } \
	static void SetInstance(_Class_* p) { coASSERT(instance == nullptr); instance = p; } \
	static void DestroyInstance() { delete instance; instance = nullptr; } \
	private: 

#define coDEFINE_SINGLETON(_Class_) 
