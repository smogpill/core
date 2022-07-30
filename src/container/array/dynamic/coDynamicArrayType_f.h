// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl_f.h>
#include "../coDynamicArray_f.h"

// TODO:
// We use a different header than coDynamicArray_f.h for these reflection macros because there are include conflicts. 
// Basically some low level headers include coDynamicArray_f.h indirectly (such as Log.h through coDynamicString).

coDEFINE_TEMPLATE_CLASS(<class T>, coDynamicArray<T>)
{
	type->writeArchiveFunc = [](coArchive& archive, const void* obj) -> coUint32
	{
		const coDynamicArray<T>& array = *static_cast<const coDynamicArray<T>*>(obj);
		if (array.count == 0)
			return 0;
		const coUint32 index = archive.GetSize();
		archive.Write(array.count);
		archive.Write(array.data, array.count * sizeof(T));
		return index;
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
