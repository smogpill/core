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

#define coDECLARE_PIMPL() \
	public: \
		class _Impl; \
		_Impl* GetImpl() const { return impl; } \
		void _OnImplConstruct(); \
		void _OnImplDestruct(); \
	private: \
		_Impl* impl

#define coDEFINE_PIMPL(_class_) \
	void _class_::_OnImplConstruct() { impl = new _Impl(); } \
	void _class_::_OnImplDestruct() { delete impl; impl = nullptr; }

#define coDECLARE_SUPER(_super_) \
	private: \
	using Super = _super_

#ifdef coREFLECT_ENABLED
#	define _coDECLARE_GET_STATIC_TYPE() static const coType* GetStaticType()
#else
#	define _coDECLARE_GET_STATIC_TYPE() static const coType* GetStaticType() { return nullptr; }
#endif

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
