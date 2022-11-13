// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/coCppExtensions.h"
#include "coTypeAutoRegistrator.h"

// TODO:
// The _Class_ parameter of coDEFINE_CLASS(_Class_) and other related functions should be unnecessary. 
// Keeping it a bit before we get all the functionalities together, and then will get rid of it.

class coType;
class coField;

using Base = void;

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

#define _coDECLARE_CLASS_SHARED(_Class_) \
	coDEFINE_ATTRIBUTE(Reflected); \
	private: \
		using Self = _Class_; \
	public: \
		static void OnInitType(coType*, coField*); \
		static coType* GetStaticType(); \
	private:

#define coDECLARE_CLASS_NO_POLYMORPHISM(_Class_) \
	private: \
		_coDECLARE_CLASS_SHARED(_Class_); \
	public: \
		coType* GetType() const { return GetStaticType(); } \
	private:

#define coDECLARE_CLASS(_Class_) \
	private: \
	_coDECLARE_CLASS_SHARED(_Class_); \
	public: \
		_coPUSH_DISABLE_OVERRIDE_WARNING() \
		virtual coType* GetType() const { return GetStaticType(); } \
		_coPOP_DISABLE_OVERRIDE_WARNING() \
	private:
