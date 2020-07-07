// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coType.h"
#include "coTypeBuilder.h"
#include "coTypeAutoRegistrator.h"
#include "../result/coResult_f.h"

#define coDEFINE_TYPE(_class_)\
class _##_class_##_typeBuilder : public coTypeBuilder \
{\
public:\
	virtual coResult OnInitType() override\
	{\
		type->name = #_class_;\
		type->nameHash = coHash32(type->name);\
		type->symbolFlags = 0;\
		type->size8 = sizeof(_class_);\
		type->alignment8 = alignof(_class_);\
		type->super = nullptr;\
		type->createFunc = []() -> void* { return new _class_(); };	\
		coTRY(OnInitMembers(), nullptr);\
		return true;\
	}\
	coResult OnInitMembers();\
};\
_##_class_##_typeBuilder co_##_class_##_typeBuilder;\
coTypeAutoRegistrator co_##_class_##_typeAutoRegistrator(co_##_class_##_typeBuilder);\
const coType* _class_::GetStaticType()\
{\
	return co_##_class_##_typeBuilder.GetType();\
}\
coResult _##_class_##_typeBuilder::OnInitMembers()
