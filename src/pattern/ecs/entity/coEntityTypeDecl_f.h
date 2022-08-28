// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coEntityType.h"
#include "lang/coCppExtensions.h"

class coEntityType;

class coFORCE_SYMBOL_INCLUSION_ATTRIBUTE coEntityTypeAutoRegistrator
{
public:
	virtual coEntityType* GetOrCreateType() = 0;
	static coEntityTypeAutoRegistrator* GetRegistrator(coUint32 idx);
	static coUint GetNbRegistrators();
	static void AddRegistrator(coEntityTypeAutoRegistrator&);
};

template <class T>
class coFORCE_SYMBOL_INCLUSION_ATTRIBUTE coClassEntityTypeAutoRegistrator : public coEntityTypeAutoRegistrator
{
public:
	coClassEntityTypeAutoRegistrator()
	{
		T::GetStaticType();
	}

	coEntityType* GetOrCreateType() override { return T::GetStaticType(); }
};



#define coDEFINE_ENTITY_TYPE(_Type_) \
	coEntityType* _Type_::GetStaticType()\
	{\
		static coEntityType* type = nullptr; \
		if (!type) \
		{ \
			type = new coEntityType(); \
			type->name = #_Type_; \
			type->nameHash = coHash32(type->name); \
			type->uid = type->nameHash; \
			OnInitType(type); \
		} \
		return type; \
	} \
	void _Type_::OnInitType(coEntityType* type)
