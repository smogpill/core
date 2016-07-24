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

#define coDECLARE_REFLECTED() \
	coDEFINE_ATTRIBUTE(Reflected, true); \
	const coType* GetType() const
