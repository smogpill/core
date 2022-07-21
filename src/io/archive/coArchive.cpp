// Copyright(c) 2019-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coArchive.h"
#include <lang/reflect/coType.h>
#include <lang/reflect/coField.h>
#include <lang/reflect/coField_f.h>
#include <container/array/coDynamicArray_f.h>

void coArchive::Clear()
{
	coClear(data);
}

void coArchive::WriteRoot(const void* object, const coType& type)
{
	Clear();

	// Offset for the root object. The value will be written later.
	PushBytes(sizeof(coUint32));

	const coUint nbSerializableFields = type.GetNbSerializableFields();

	// Vtable
	const coUint32 vtableIndex = data.count;
	{
		const coUint vtableSize = (2 + nbSerializableFields) * sizeof(coUint16);
		coASSERT(vtableSize <= coUint16(-1));
		const coUint16 vtableSize16 = coUint16(vtableSize);
		// Content: 
		// - size of vtable: coUint16
		// - size of inline data: coUint16
		// - Offsets to fields: coUint16 x nbSerializableFields
		PushBytes(vtableSize16);
		coUint16* vtable = reinterpret_cast<coUint16*>(&data.data[vtableIndex]);
		vtable[0] = vtableSize16;

		coUint32 serializableFieldIdx = 0;
		coUint32 inlineDataSize = 4;
		for (const coField* field : type.fields)
		{
			if (field->IsSerializable())
			{
				coASSERT(inlineDataSize <= coUint16(-1));
				vtable[2 + serializableFieldIdx] = coUint16(inlineDataSize);
				inlineDataSize += field->type->size8;
			}
		}
		coASSERT(inlineDataSize <= coUint16(-1));
		vtable[1] = coUint16(inlineDataSize);
	}

	// Update the root address
	*((coUint32*)data.data) = data.count;

	// Content of the object
	{
		const coUint16 inlineDataSize = reinterpret_cast<coUint16*>(&data.data[vtableIndex])[1];
		const coUint32 inlineDataIdx = data.count;
		coUint32 inlineItIdx = inlineDataIdx + 4;
		PushBytes(inlineDataSize);
		*(coInt32*)(&data.data[inlineDataIdx]) = coInt32(data.count - vtableIndex); // Offset from vtable
		coUint32 serializableFieldIdx = 0;
		for (const coField* field : type.fields)
		{
			if (field->IsSerializable())
			{
				const coType* fieldType = field->type;
				coASSERT(reinterpret_cast<coUint16*>(&data.data[vtableIndex])[2 + serializableFieldIdx++] == inlineItIdx - inlineDataIdx);
				coMemCopy(&data.data[inlineItIdx], ((coByte*)object) + field->offset8, fieldType->size8);
				inlineItIdx += fieldType->size8;
			}
		}
	}
	
}

void coArchive::Write(const void* buffer, coUint32 size)
{
	coPushBackArray(data, coArray<const coByte>(static_cast<const coByte*>(buffer), size));
}

void coArchive::PushBytes(coUint size)
{
	coResize(data, size);
}
