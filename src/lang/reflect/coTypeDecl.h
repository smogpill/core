// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"

class coType;

//#ifdef coREFLECTION_PARSING
#define coDEFINE_ATTRIBUTE(_attr_, ...) struct coMETA(__VA_ARGS__) _attribute_##_attr_ {}
/*#else
#define coDEFINE_ATTRIBUTE(_attr_, ...)
#endif*/

#ifdef coCLANG_COMPILER
#	define _coPUSH_DISABLE_OVERRIDE_WARNING() _Pragma("clang diagnostic push") \
	_Pragma("clang diagnostic ignored \"-Winconsistent-missing-override\"")
#	define _coPOP_DISABLE_OVERRIDE_WARNING() _Pragma("clang diagnostic pop")
#else
#	define _coPUSH_DISABLE_OVERRIDE_WARNING()
#	define _coPOP_DISABLE_OVERRIDE_WARNING()
#endif

#define coDECLARE_BASE(_Base_) \
	private: \
	using Super = _Base_; \
	using Base = _Base_

#define coDECLARE_SUPER(_Super_) coDECLARE_BASE(_Super_)

#define _coDECLARE_GET_STATIC_TYPE() static const coType* GetStaticType()

#define _coDECLARE_REFLECTED_SHARED() \
	coDEFINE_ATTRIBUTE(Reflected); \
	public: \
		_coDECLARE_GET_STATIC_TYPE(); \
	private:

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
		_coPUSH_DISABLE_OVERRIDE_WARNING() \
		virtual const coType* GetType() const { return GetStaticType(); } \
		_coPOP_DISABLE_OVERRIDE_WARNING() \
	private:
