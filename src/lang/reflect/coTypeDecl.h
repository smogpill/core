// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coType;

#ifdef coREFLECTION_PARSING
#define coDECLARE_ATTRIBUTE(_attr_) struct coMETA(coAttribute##_attr_) _attribute_##__LINE__ {}
#else
#define coDECLARE_ATTRIBUTE(_attr_)
#endif

#define coDECLARE_TYPE(_type_) \
	using Self = _type_

#define coDECLARE_DERIVED(_super_) \
	using Super = _super_

#define coDECLARE_ROOT_TYPE(_type_) \
	coDECLARE_TYPE(_type_)

#define coDECLARE_DERIVED_TYPE(_type_, _super_) \
	coDECLARE_TYPE(_type_); \
	coDECLARE_DERIVED(_super_)

#define coDECLARE_REFLECTED(_type_) \
	coDECLARE_ATTRIBUTE(Reflected); \
	struct h {}; \
	const coType* GetType() const
