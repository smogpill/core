// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl_f.h>
#include "../coDynamicArray_f.h"

// TODO:
// We use a different header than coDynamicArray_f.h because of very low level headers that include dynamic arrays 
// (such as Log.h through coDynamicString)

coDEFINE_TEMPLATE_CLASS(<class T>, coDynamicArray<T>)
{
	type->writeArchiveFunc = [](coArchive& archive, const void* obj)
	{
		const coDynamicArray<T>& array = *static_cast<const coDynamicArray<T>*>(obj);
		archive.WriteBuffer(&array.count, sizeof(array.count));
		archive.WriteBuffer(array.data, array.count * sizeof(T));
	};
	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coDynamicArray<T>& array = *static_cast<coDynamicArray<T>*>(obj);
		coClear(array);
		const coUint32 count = archive.Get<coUint32>(idx);
		coResize(array, count);
		archive.ReadBuffer(idx + sizeof(coUint32), array.data, count * sizeof(T));
	};
}
