// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "lang/pch.h"
#include "lang/reflect/coTypeBuilder.h"
#include "lang/reflect/coType.h"
#include "lang/result/coResult_f.h"

// class T : public coTypeBuilder
// {
// public:
// 	virtual coResult OnInitType() override
// 	{
// 		return true;
// 	}
// 	virtual coResult OnLinkType() override
// 	{
// 		return true;
// 	}
// };
// 
// #include "lang/reflect/coTypeAutoRegistrator.h"
// 
// T t;
// coTypeAutoRegistrator a(t);
// 
// coFORCE_SYMBOL_INCLUSION(a);

coTypeBuilder::coTypeBuilder()
	: type(new coType())
{
	type->builder = this;
}

coTypeBuilder::~coTypeBuilder()
{
	delete type;
}

void coTypeBuilder::Clear()
{
	delete type;
	type = new coType();
}

coResult coTypeBuilder::InitType()
{
	coTRY(OnInitType(), "Failed to init the type: " << type->name);
	return true;
}

coResult coTypeBuilder::LinkType()
{
	coTRY(type, nullptr);
	coTRY(OnLinkType(), "Failed to link the type: " << type->name);
	return true;
}
