// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#define coDECLARE_SINGLETON(_className_) \
	public: \
	inline static _className_* instance = nullptr; \
	static void SetInstance(_className_* p) { coASSERT(instance == nullptr); instance = p; } \
	static void DestroyInstance() { delete instance; instance = nullptr; } \
	private: 

#define coDEFINE_SINGLETON(_className_) 
