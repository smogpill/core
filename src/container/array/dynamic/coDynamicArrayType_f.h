// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl_f.h>
#include <io/archive/coArchive.h>
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
		const coType* type = coTypeHelper<T>::GetStaticType();
		if (type && !type->triviallySerializable)
		{
			coUint32 offset = archive.GetSize();
			archive.PushBytes(array.count * sizeof(coUint32));
			const coUint32 vtableIdx = archive.WriteVTable(*type);
			for (coUint32 elIdx = 0; elIdx < array.count; ++elIdx)
			{
				const coUint32 objIdx = archive.WriteObject(&array.data[elIdx], *type, vtableIdx);
				archive.SetValueAtOffset(offset, objIdx);
				offset += sizeof(coUint32);
			}
		}
		else
		{
			archive.Write(array.data, array.count * sizeof(T));
		}
		return index;
	};
	type->readArchiveFunc = [](const coArchive& archive, coUint32 idx, void* obj)
	{
		coDynamicArray<T>& array = *static_cast<coDynamicArray<T>*>(obj);
		coClear(array);
		const coUint32 count = archive.Get<coUint32>(idx);
		coResize(array, count);
		const coType* type = coTypeHelper<T>::GetStaticType();
		if (type && !type->triviallySerializable)
		{
			const coUint32* offsets = reinterpret_cast<const coUint32*>(&archive.GetData()[idx + sizeof(coUint32)]);
			for (coUint32 elIdx = 0; elIdx < count; ++elIdx)
			{
				const coUint32 objIdx = offsets[elIdx];
				if (objIdx)
					archive.ReadObject(objIdx, &array.data[elIdx], *type);
			}
		}
		else
		{
			archive.ReadBuffer(idx + sizeof(coUint32), array.data, count * sizeof(T));
		}
	};
}
