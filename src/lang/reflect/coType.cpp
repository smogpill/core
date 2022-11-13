// Copyright(c) 2016-2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "lang/pch.h"
#include "lang/reflect/coType.h"
#include "lang/reflect/coField.h"
#include "lang/reflect/coFunction.h"
#include "container/array/coDynamicArray_f.h"
#include "coTypeRegistry.h"

coType::coType()
	: triviallyCopyable(false)
	, triviallySerializable(false)
{
	coTypeRegistry::CreateInstanceIfMissing();
	coTypeRegistry::instance->Add(*this);
}

coType::~coType()
{
	for (coField* p : fields)
		delete p;
	for (auto p : functions)
		delete p;
}

void coType::Give(coField& field)
{
	coPushBack(fields, &field);
}

void coType::AddDependency(coType& type)
{
	if (!coContains(dependencies, &type))
		coPushBack(dependencies, &type);
}

coUint coType::GetNbSerializableFields() const
{
	coUint nb = 0;
	for (const coField* f : fields)
		nb += f->IsSerializable();
	return nb;
}

coBool coType::IsCompatibleWith(const coType& type) const
{
	const coUint32 expectedUid = type.uid;
	const coType* it = this;
	do
	{
		if (it->uid == expectedUid)
			return true;
		it = it->base;
	} while (it);
	return false;
}

void coType::Init(coType* type)
{
	if (type == nullptr)
		type = this;
	if (base)
		base->Init(type);
	if (initTypeFunc)
		initTypeFunc(type, nullptr);
}
