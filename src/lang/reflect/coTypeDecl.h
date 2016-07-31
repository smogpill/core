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

#ifdef coREFLECT_ENABLED
#	define _coDECLARE_GET_STATIC_TYPE() static const coType* GetStaticType()
#else
#	define _coDECLARE_GET_STATIC_TYPE() static const coType* GetStaticType() { return nullptr; }
#endif

#define _coDECLARE_REFLECTED_SHARED() \
	coDEFINE_ATTRIBUTE(Reflected, true); \
	_coDECLARE_GET_STATIC_TYPE()

#define coDECLARE_REFLECTED_NO_VIRTUAL() \
	private: \
	_coDECLARE_REFLECTED_SHARED(); \
	public: \
		const coType* GetType() const { return GetStaticType(); } \
	private:

#define coDECLARE_REFLECTED_VIRTUAL() \
	private: \
	_coDECLARE_REFLECTED_SHARED(); \
	public: \
		virtual const coType* GetType() const { return GetStaticType(); } \
	private:
