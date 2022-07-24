// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coType.h"
#include "coTypeAutoRegistrator.h"
#include "../result/coResult_f.h"

template <class T>
struct coTypeHelper
{
	static coType* GetStaticType() { return T::GetStaticType(); }
};

template <>
struct coTypeHelper<void>
{
	static coType* GetStaticType() { return nullptr; }
};

struct _coNoReflectType
{
	static coType* GetStaticType() { return nullptr; }
};

template <class T, class = std::void_t<>>
struct _coReflectCheck : std::false_type {};

template <typename T>
struct _coReflectCheck<T, std::void_t<typename T::_attribute_Reflected>> : std::true_type {};

template <class T>
coType* coGetType()
{
	return coTypeHelper<std::conditional<_coReflectCheck<T>::value, T, _coNoReflectType>::type>::GetStaticType();
}

#define coDEFINE_FIELD(_name_) \
	field = new coField(); \
	field->name = #_name_; \
	field->nameHash = coHash32(field->name); \
	field->uid = field->nameHash; \
	field->type = coTypeHelper<decltype(Class::_name_)>::GetStaticType(); \
	field->offset8 = static_cast<decltype(field->offset8)>(coGetFieldOffset<Class>(&Class::_name_)); \
	type->Give(*field);

#define coDEFINE_CLASS(_Class_) \
coClassTypeAutoRegistrator<_Class_> co_##_Class_##_typeAutoRegistrator;\
coType* _Class_::GetStaticType()\
{\
	static coType* type = nullptr; \
	if (!type) \
	{ \
		type = new coType(); \
		type->name = #_Class_; \
		type->nameHash = coHash32(type->name); \
		type->uid = type->nameHash; \
		type->size8 = sizeof(_Class_); \
		type->alignment8 = alignof(_Class_); \
		type->createFunc = []() -> void* { return new _Class_(); };	\
		type->super = coTypeHelper<Base>::GetStaticType(); \
		OnInitType<_Class_>(type, nullptr); \
	} \
	return type; \
}\
template <class Class> void _Class_::OnInitType(coType* type, coField* field)

#define coDECLARE_FUNDAMENTAL_TYPE(_Type_) \
	template <> \
	struct coTypeHelper<_Type_>\
	{ \
		static coType* GetStaticType() \
		{ \
			static coType* type = nullptr; \
			if (!type) \
			{ \
				type = new coType(); \
				type->name = #_Type_; \
				type->nameHash = coHash32(type->name); \
				type->uid = type->nameHash; \
				type->size8 = sizeof(_Type_); \
				type->alignment8 = alignof(_Type_); \
				type->createFunc = []() -> void* { return new _Type_(); }; \
			} \
			return type; \
		} \
	}

coDECLARE_FUNDAMENTAL_TYPE(coBool);
coDECLARE_FUNDAMENTAL_TYPE(coInt8);
coDECLARE_FUNDAMENTAL_TYPE(coInt16);
coDECLARE_FUNDAMENTAL_TYPE(coInt32);
coDECLARE_FUNDAMENTAL_TYPE(coInt64);
coDECLARE_FUNDAMENTAL_TYPE(coUint8);
coDECLARE_FUNDAMENTAL_TYPE(coUint16);
coDECLARE_FUNDAMENTAL_TYPE(coUint32);
coDECLARE_FUNDAMENTAL_TYPE(coUint64);
coDECLARE_FUNDAMENTAL_TYPE(coFloat);
coDECLARE_FUNDAMENTAL_TYPE(coFloat64);
coDECLARE_FUNDAMENTAL_TYPE(coNullPtr);
