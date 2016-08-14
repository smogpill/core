// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "lang/pch.h"
#include "lang/types/coBaseTypes.h"
#include "lang/reflect/coTypeBuilder.h"
#include "lang/reflect/coTypeAutoRegistrator.h"
#include "lang/reflect/coType.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coTypeDecl.h"

template <class T>
class coBaseType_typeBuilder : public coTypeBuilder
{
	coDECLARE_SUPER(coTypeBuilder);
public:
	coBaseType_typeBuilder(const coConstString& _typeName)
		: typeName(_typeName)
	{
	}

protected:
	virtual coResult OnInitType() override
	{
		coTRY(Super::OnInitType(), nullptr);
		type->name = typeName;
		type->nameHash = coHash32(type->name);
		type->size8 = sizeof(T);
		type->alignment8 = alignof(T);
		type->createFunc = []() -> void* { return new T(); };

		return true;
	}
private:
	coConstString typeName;
};

#define coDEFINE_BASE_TYPE(_type_) \
	coBaseType_typeBuilder<_type_> co_typeBuilder_##_type_(#_type_); \
	const coType* _coGetStaticType_##_type_() { return co_typeBuilder_##_type_.GetType(); } \
	coTypeAutoRegistrator co_typeAutoRegistrator_##_type_(co_typeBuilder_##_type_)

coDEFINE_BASE_TYPE(coBool);
coDEFINE_BASE_TYPE(coByte);
coDEFINE_BASE_TYPE(coChar);
coDEFINE_BASE_TYPE(coWideChar);
coDEFINE_BASE_TYPE(coInt8);
coDEFINE_BASE_TYPE(coInt16);
coDEFINE_BASE_TYPE(coInt32);
coDEFINE_BASE_TYPE(coInt64);
coDEFINE_BASE_TYPE(coUint8);
coDEFINE_BASE_TYPE(coUint16);
coDEFINE_BASE_TYPE(coUint32);
coDEFINE_BASE_TYPE(coUint64);
coDEFINE_BASE_TYPE(coFloat);
coDEFINE_BASE_TYPE(coFloat64);
coDEFINE_BASE_TYPE(coNullPtr);
coDEFINE_BASE_TYPE(coIntPtr);
coDEFINE_BASE_TYPE(coUintPtr);
