// Copyright(c) 2016-2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "lang/pch.h"
#include "lang/reflect/coType.h"
#include "lang/reflect/coField.h"
#include "lang/reflect/coFunction.h"
#include "container/array/coDynamicArray_f.h"

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

coUint coType::GetNbSerializableFields() const
{
	coUint nb = 0;
	for (const coField* f : fields)
		nb += f->IsSerializable();
	return nb;
}
