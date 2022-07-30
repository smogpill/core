// Copyright(c) 2020-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coType.h"
#include "coField.h"
#include "coField_f.h"
#include "coTypeAutoRegistrator.h"
#include "../result/coResult_f.h"

template <class T, class = void>
struct coTypeHelper
{
	static coType* GetStaticType() { return nullptr; }
};

// See https://en.cppreference.com/w/cpp/types/void_t for more info on how that works
template <class T>
struct coTypeHelper<T, std::void_t<decltype(T::GetStaticType)>>
{
	static coType* GetStaticType() { return T::GetStaticType(); }
};

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
	field->pointer = std::is_pointer<decltype(Class::_name_)>::value; \
	field->type = coTypeHelper<std::remove_pointer<decltype(Class::_name_)>::type>::GetStaticType(); \
	field->offset8 = static_cast<decltype(field->offset8)>(coGetFieldOffset<Class>(&Class::_name_)); \
	type->Give(*field);

#define coDEFINE_VIRTUAL_FIELD(_name_) \
	field = new coField(); \
	field->name = #_name_; \
	field->nameHash = coHash32(field->name); \
	field->uid = field->nameHash; \
	type->Give(*field);

#define _coDEFINE_AUTO_REGISTRATOR(_Class_) \
	coClassTypeAutoRegistrator<_Class_> coCONCAT(co_typeAutoRegistrator, __COUNTER__)

#define _coDEFINE_GET_STATIC_TYPE(_Class_) \
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
			type->triviallyCopyable = std::is_trivially_copyable<_Class_>::value; \
			OnInitType<_Class_>(type, nullptr); \
		} \
		return type; \
	}

#define _coDEFINE_ON_INIT_TYPE(_Class_) \
	template <class Class> void _Class_::OnInitType(coType* type, coField* field)

#define coDEFINE_CLASS(_Class_) \
	_coDEFINE_AUTO_REGISTRATOR(_Class_); \
	_coDEFINE_GET_STATIC_TYPE(_Class_); \
	_coDEFINE_ON_INIT_TYPE(_Class_)

#define coDEFINE_TEMPLATE_CLASS(_TemplateParams_, _Class_) \
	template _TemplateParams_ \
	_coDEFINE_GET_STATIC_TYPE(_Class_); \
	template _TemplateParams_ \
	_coDEFINE_ON_INIT_TYPE(_Class_)

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
				type->triviallyCopyable = true; \
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
