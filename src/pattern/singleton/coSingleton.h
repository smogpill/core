// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#define coDECLARE_SINGLETON(_className_) \
	public: \
	static _className_* instance; \
	static void SetInstance(_className_*); \
	static void DestroyInstance(); \
	private: 

#define coDEFINE_SINGLETON(_className_) \
	_className_* _className_::instance = nullptr; \
	void _className_::SetInstance(_className_* p) \
	{ \
		instance = p; \
	} \
	void _className_::DestroyInstance() \
	{ \
		delete instance; \
		instance = nullptr; \
	}
