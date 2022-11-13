// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coArchiveWriter.h"
#include <lang/reflect/coType.h>
#include <lang/reflect/coField.h>
#include <container/array/dynamic/coReverseDynamicArray_f.h>

void coArchiveWriter::Clear()
{
	coClear(data);
}

void coArchiveWriter::WriteRoot(const void* object, const coType& type)
{
	Clear();

	/*

	for (const coField* field : type.fields)
	{
		if (field->IsSerializable())
		{
		}
	}


	// Offset for the root object. The value will be written later.
	PushBytes(sizeof(coUint32));

	const coUint nbSerializableFields = type.GetNbSerializableFields();

	// Vtable
	const coUint32 vtableIndex = data.count;
	{
		const coUint16 vtableSize = (2 + nbSerializableFields) * sizeof(coUint16);
		// Content: 
		// - size of vtable: coUint16
		// - size of inline data: coUint16
		// - Offsets to fields: coUint16 x nbSerializableFields
		PushBytes(vtableSize);
		*reinterpret_cast<coUint16*>(&data.data[vtableIndex]) = vtableSize;
	}

	const coUint32 inlineStart = data.count;

	Write(coInt32(inlineStart - vtableIndex)); // Vtable offset
	coUint16 inlineSize = 4;
	for (const coField* field : type.fields)
	{
		if (field->IsSerializable())
		{
			const coType* fieldType = field->type;
			inlineSize += fieldType->size8;
		}
	}

	const coUint32 inlineSize = data.count - inlineStart;
	coASSERT(inlineSize <= coUint16(-1));
	*reinterpret_cast<coUint16*>(&data.data[vtableIndex + sizeof(coUint16)]) = coUint16(inlineSize);
	* */
}

void coArchiveWriter::Write(const void* buffer, coUint32 size)
{
	coPushBackArray(data, coArray<const coByte>(static_cast<const coByte*>(buffer), size));
}

void coArchiveWriter::PushBytes(coUint size)
{
	coResize(data, size);
}
