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

using Base = void;

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

#define _coDECLARE_CLASS_SHARED() \
	coDEFINE_ATTRIBUTE(Reflected); \
	public: \
		static coType* GetStaticType(); \
	private:

#define coDECLARE_CLASS_NO_POLYMORPHISM() \
	private: \
		_coDECLARE_CLASS_SHARED(); \
	public: \
		coType* GetType() const { return GetStaticType(); } \
	private:

#define coDECLARE_CLASS() \
	private: \
	_coDECLARE_CLASS_SHARED(); \
	public: \
		_coPUSH_DISABLE_OVERRIDE_WARNING() \
		virtual coType* GetType() const { return GetStaticType(); } \
		_coPOP_DISABLE_OVERRIDE_WARNING() \
	private:
