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

template <class T, coBool>
struct _coTypeFactory
{
};

template <class T>
struct _coTypeFactory<T, false>
{
	static void* Create() { return new T(); }
	static void* CopyCreate(const void* from) { return new T(*static_cast<const T*>(from)); }
	static void Move(const void* from, void* to) { *static_cast<T*>(to) = std::move(*static_cast<const T*>(from)); }
	static void Construct(void* p) { coASSERT(p); new (p) T(); }
	static void Destruct(void* p) { coASSERT(p); static_cast<T*>(p)->~T(); }
};

template <class T>
struct _coTypeFactory<T, true>
{
	static void* Create() { return nullptr; }
	static void* CopyCreate(const void*) { return nullptr; }
	static void Move(const void*, void*) {}
	static void Construct(void* p) {}
	static void Destruct(void* p) {}
};

template <class T>
void* coCreate()
{
	return _coTypeFactory<T, std::is_abstract<T>::value>::Create();
}

template <class T>
void* coCopyCreate(const void* from)
{
	return _coTypeFactory<T, std::is_abstract<T>::value>::CopyCreate(from);
}


template <class T>
void coMove(const void* from, void* to)
{
	_coTypeFactory<T, std::is_abstract<T>::value>::Move(from, to);
}

template <class T>
void coConstruct(void* p)
{
	_coTypeFactory<T, std::is_abstract<T>::value>::Construct(p);
}

template <class T>
void coDestruct(void* p)
{
	_coTypeFactory<T, std::is_abstract<T>::value>::Destruct(p);
}

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
	field->pointer = std::is_pointer<decltype(Self::_name_)>::value; \
	field->type = coTypeHelper<std::remove_pointer<decltype(Self::_name_)>::type>::GetStaticType(); \
	field->offset8 = static_cast<decltype(field->offset8)>(coGetFieldOffset<Self>(&Self::_name_)); \
	type->Give(*field);

#define coDEFINE_VIRTUAL_FIELD(_name_, _Type_) \
	field = new coField(); \
	field->name = #_name_; \
	field->nameHash = coHash32(field->name); \
	field->uid = field->nameHash; \
	field->type = coTypeHelper<_Type_>::GetStaticType(); \
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
			type->createFunc = &coCreate<_Class_>; \
			type->copyCreateFunc = &coCopyCreate<_Class_>; \
			type->moveFunc = &coMove<_Class_>; \
			type->constructFunc = &coConstruct<_Class_>; \
			type->destructFunc = &coDestruct<_Class_>; \
			type->base = coTypeHelper<Base>::GetStaticType(); \
			type->triviallyCopyable = std::is_trivially_copyable<_Class_>::value; \
			OnInitType(type, nullptr); \
		} \
		return type; \
	}

#define _coDEFINE_ON_INIT_TYPE(_Class_) \
	void _Class_::OnInitType(coType* type, coField* field)

#define coDEFINE_CLASS(_Class_) \
	_coDEFINE_AUTO_REGISTRATOR(_Class_); \
	_coDEFINE_GET_STATIC_TYPE(_Class_); \
	_coDEFINE_ON_INIT_TYPE(_Class_)

#define coBEGIN_CLASS(_Class_) \
	coDEFINE_CLASS(_Class_) \
	{
#define coEND_CLASS() \
	}

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
				type->createFunc = &coCreate<_Type_>; \
				type->copyCreateFunc = &coCopyCreate<_Type_>; \
				type->moveFunc = &coMove<_Type_>; \
				type->constructFunc = &coConstruct<_Type_>; \
				type->destructFunc = &coDestruct<_Type_>; \
				type->triviallyCopyable = true; \
				type->triviallySerializable = true; \
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
