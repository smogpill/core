// Copyright(c) 2019-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coArchive.h"
#include <lang/reflect/coType_f.h>
#include <lang/reflect/coField.h>
#include <lang/reflect/coField_f.h>
#include <lang/reflect/coTypeRegistry.h>
#include <container/array/coDynamicArray_f.h>
#include <math/scalar/coUint32_f.h>

/// # Todo
/// - Blob/Stream fields where the serialization is done by callbacks with streams. To compare to virtual fields to see how much they make sense.
/// 
/// # Notes
/// - The data is often aligned to a sizeof(coUint32) during the serialization. Working on unaligned int are not an issue on x86/x64, but it seems to be slow or even crashes on some ARM platforms. 

void coArchive::Clear()
{
	coClear(data);
}

void coArchive::WriteRoot(const void* object, const coType& type)
{
	Clear();

	// Offset of the root index
	PushBytes(sizeof(coUint32));

	// We write objects recursively
	const coUint32 rootIdx = WriteObject(object, type);

	// We write the index of the root object
	*((coUint32*)data.data) = rootIdx;
}

void coArchive::PushToAlignment32()
{
	const coUint remaining = data.count % sizeof(coUint32);
	PushBytes(remaining);
}

coUint32 coArchive::WriteVTable(const coType& type)
{
	PushToAlignment32();
	const coUint32 vtableIndex = data.count;

	// Content: 
	// - size of vtable: coUint16
	// - size of inline data: coUint16
	// - uid of the type: coUint32
	// - Offsets to fields: coUint16 x nbSerializableFields

	const coUint nbSerializableFields = type.GetNbSerializableFields();
	const coUint vtableSize = (4 + nbSerializableFields) * sizeof(coUint16);
	coASSERT(vtableSize <= coUint16(-1));
	const coUint16 vtableSize16 = coUint16(vtableSize);
	PushBytes(vtableSize16);
	coUint16* vtable = reinterpret_cast<coUint16*>(&data.data[vtableIndex]);

	// Size of the vtable
	vtable[0] = vtableSize16;

	// Uid of the type
	*((coUint32*)&vtable[2]) = type.uid;

	// Iterate on serializable fields
	coUint32 serializableFieldIdx = 0;
	coUint32 inlineDataSize = 4;
	for (const coField* field : type.fields)
	{
		if (field->IsSerializable())
		{
			coASSERT(field->type);

			// Offset to the inline data
			coASSERT(inlineDataSize <= coUint16(-1));
			vtable[4 + serializableFieldIdx] = coUint16(inlineDataSize);

			inlineDataSize += field->type->triviallySerializable ? field->type->size8 : sizeof(coUint32);
			++serializableFieldIdx;
		}
	}

	// Alignment
	inlineDataSize += inlineDataSize % sizeof(coUint32);

	// Size of the inline data
	coASSERT(inlineDataSize <= coUint16(-1));
	vtable[1] = coUint16(inlineDataSize);

	return vtableIndex;
}

coUint32 coArchive::WriteObject(const void* object, const coType& type)
{
	if (object == nullptr)
		return 0;

	if (type.writeArchiveFunc)
	{
		return type.writeArchiveFunc(*this, object);
	}

	const coUint32 vtableIndex = WriteVTable(type);
	return WriteObject(object, type, vtableIndex);
}

coUint32 coArchive::WriteObject(const void* object, const coType& type, coUint32 vtableIndex)
{
	if (object == nullptr)
		return 0;

	PushToAlignment32();

	if (type.writeArchiveFunc)
	{
		return type.writeArchiveFunc(*this, object);
	}

	// Object
	const coUint32 objectIdx = data.count;
	{
		// Content: 
		// - Offset to the vtable (without the sign): coInt32
		// - Inline data: type of the inline data*

		const coUint16 inlineDataSize = reinterpret_cast<coUint16*>(&data.data[vtableIndex])[1];
		PushBytes(inlineDataSize);
		*(coInt32*)(&data.data[objectIdx]) = coInt32(objectIdx - vtableIndex); // Offset from vtable
		coUint32 inlineItIdx = objectIdx + 4;
		coUint32 serializableFieldIdx = 0;
		for (const coField* field : type.fields)
		{
			if (field->IsSerializable())
			{
				const coType* fieldType = field->type;
				coASSERT(fieldType);
				coASSERT(reinterpret_cast<coUint16*>(&data.data[vtableIndex])[4 + serializableFieldIdx] == inlineItIdx - objectIdx);
				if (fieldType->triviallySerializable)
				{
					coMemCopy(&data.data[inlineItIdx], ((coByte*)object) + field->offset8, fieldType->size8);
					inlineItIdx += fieldType->size8;
				}
				else
				{
					coUint32 offset;
					if (field->pointer)
					{
						const void* fieldObject;
						coMemCopy(&fieldObject, static_cast<const coUint8*>(object) + field->offset8, sizeof(fieldObject));
						const coUint32 index = WriteObject(fieldObject, *fieldType);
						coASSERT(index == 0 || (index >= objectIdx + inlineDataSize));
						offset = index ? (index - inlineItIdx) : 0u;
					}
					else
					{
						const coUint32 index = WriteObject(static_cast<const coUint8*>(object) + field->offset8, *fieldType);
						coASSERT(index == 0 || (index >= objectIdx + inlineDataSize));
						offset = index ? (index - inlineItIdx) : 0u;
					}

					coMemCopy(&data.data[inlineItIdx], &offset, sizeof(coUint32));
					inlineItIdx += sizeof(coUint32);
				}

				++serializableFieldIdx;
			}
		}
	}
	return objectIdx;
}

void* coArchive::CreateObjects(const coType& expectedRootBaseType) const
{
	return CreateObjects(GetRoot(), expectedRootBaseType);
}

void* coArchive::CreateObjects(coUint32 objectIdx, const coType& expectedBaseType) const
{
	const coType* type = ReadObjectType(objectIdx);
	if (type == nullptr)
		return nullptr;

	if (!type->IsCompatibleWith(expectedBaseType))
		return nullptr;

	void* object = type->createFunc();
	coASSERT(object);
	ReadObject(objectIdx, object, *type);
	return object;
}

const coType* coArchive::ReadObjectType(coUint32 objectIdx) const
{
	const coInt32 vtableOffset = -Get<coInt32>(objectIdx);
	const coUint16* vtable = reinterpret_cast<const coUint16*>(&data.data[objectIdx + vtableOffset]);
	const coUint16 vtableSize = vtable[0];
	const coUint16 inlineSize = vtable[1];
	const coUint32 typeUID = *((const coUint32*)&vtable[2]);
	coTypeRegistry* typeRegistry = coTypeRegistry::instance;
	coASSERT(typeRegistry);
	return typeRegistry->Get(typeUID);
}

coUint32 coArchive::GetRoot() const
{
	return data.count >= 4 ? Get<coUint32>(0) : coUint32(-1);
}

void coArchive::ReadObject(coUint32 objectIdx, void* object, const coType& type) const
{
	if (type.readArchiveFunc)
	{
		type.readArchiveFunc(*this, objectIdx, object);
		return;
	}

	const coInt32 vtableOffset = -Get<coInt32>(objectIdx);
	const coUint16* vtable = reinterpret_cast<const coUint16*>(&data.data[objectIdx + vtableOffset]);

	const coUint16* inlineOffsets = &vtable[4];
	coUint serializableFieldIdx = 0;
	for (const coField* field : type.fields)
	{
		if (field->IsSerializable())
		{
			const coType* fieldType = field->type;
			coASSERT(fieldType);
			const coUint16 inlineOffset = inlineOffsets[serializableFieldIdx];
			if (fieldType->triviallySerializable)
			{
				coASSERT(objectIdx + inlineOffset + field->type->size8 <= data.count);
				const void* fieldData = &data.data[objectIdx + inlineOffset];
				coMemCopy(((coUint8*)object) + field->offset8, fieldData, field->type->size8);
			}
			else
			{
				coASSERT(objectIdx + inlineOffset + sizeof(coUint32) <= data.count);
				const coUint32 fieldOffset = *reinterpret_cast<const coUint32*>(&data.data[objectIdx + inlineOffset]);
				if (fieldOffset)
				{
					const coUint32 fieldIdx = objectIdx + inlineOffset + fieldOffset;
					coASSERT(fieldIdx < data.count);
					if (field->pointer)
					{
						void* fieldObject = CreateObjects(fieldIdx, *fieldType);
						coMemCopy(((coUint8*)object) + field->offset8, &fieldObject, sizeof(fieldObject));
					}
					else if (fieldType->readArchiveFunc)
					{
						fieldType->readArchiveFunc(*this, fieldIdx, ((coUint8*)object) + field->offset8);
					}
					else
					{
						coASSERT(false);
					}
				}
			}
			++serializableFieldIdx;
		}
	}
}

void coArchive::Write(const void* buffer, coUint32 size)
{
	coPushBackArray(data, coArray<const coByte>(static_cast<const coByte*>(buffer), size));
}

void coArchive::PushBytes(coUint size)
{
	coResize(data, data.count + size);
}

coBool coArchive::IsFieldInlinable(const coField& field)
{
	return true;
}

void coArchive::Read(void* buffer, coUint32 size) const
{

}

void coArchive::ReadBuffer(coUint32 idx, void* buffer, coUint32 size) const
{
	coASSERT(idx + size <= data.count);
	coMemCopy(buffer, &data[idx], size);
}