// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"

class coType;

#ifdef coREFLECTION_PARSING
#define _coUNIQUE(_unique_) _unique_
#define coDEFINE_ATTRIBUTE(_attr_, ...) struct coMETA(__VA_ARGS__) _attribute_##_attr_ {}
#else
#define coDEFINE_ATTRIBUTE(_attr_, ...)
#endif

#define coDECLARE_SUPER(_super_) \
	private: \
	using Super = _super_

#define _coDECLARE_REFLECTED_SHARED() \
	coDEFINE_ATTRIBUTE(Reflected, true); \
	static coType* CreateType(); \
	static coType* staticType

#ifdef coREFLECT_ENABLED
#define _coSTATIC_TYPE_ACCESS() staticType
#else
#define _coSTATIC_TYPE_ACCESS() nullptr
#endif

#define coDECLARE_REFLECTED_NO_VIRTUAL() \
	private: \
	_coDECLARE_REFLECTED_SHARED(); \
	public: \
		const coType* GetType() const { return _coSTATIC_TYPE_ACCESS(); } \
	private:

#define coDECLARE_REFLECTED_VIRTUAL() \
	private: \
	_coDECLARE_REFLECTED_SHARED(); \
	public: \
		virtual const coType* GetType() const { return _coSTATIC_TYPE_ACCESS(); } \
	private:
