// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coType;

#ifdef coREFLECTION_PARSING
#define _coUNIQUE(_unique_) _unique_
#define coDEFINE_ATTRIBUTE(_attr_, ...) struct coMETA(__VA_ARGS__) _attribute_##_attr_ {}
#else
#define coDEFINE_ATTRIBUTE(_attr_, ...)
#endif

#define coDECLARE_TYPE(_type_) \
	using Self = _type_

#define coDECLARE_SUPER(_super_) \
	using Super = _super_

#define coDECLARE_ROOT_TYPE(_type_) \
	coDECLARE_TYPE(_type_)

#define coDECLARE_DERIVED_TYPE(_type_, _super_) \
	coDECLARE_TYPE(_type_); \
	coDECLARE_SUPER(_super_)

#define _coDECLARE_REFLECTED_SHARED() \
	coDEFINE_ATTRIBUTE(Reflected, true)

#define coDECLARE_REFLECTED_NO_VIRTUAL() \
	_coDECLARE_REFLECTED_SHARED(); \
	public: \
		const coType* GetType() const; \
	private:

#define coDECLARE_REFLECTED() \
	_coDECLARE_REFLECTED_SHARED(); \
	public: \
		virtual const coType* GetType() const; \
	private:
