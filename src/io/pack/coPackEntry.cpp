// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coPackEntry.h"
#include "coPack.h"
#include <container/array/coDynamicArray_f.h>
#include <container/string/coConstString.h>
#include <container/string/coDynamicString_f.h>

coPackEntry coPackEntry::PushEntry(const coPackFormat& format_)
{
	coUint32 startOffset = pack.buffer.count;
	const coUint16 vtableSize = 2 * (2 + format_.nbFields);
	const coUint16 tableSize = 4 + format_.totalSize;
	if (startOffset == 0)
	{
		coResize(pack.buffer, 4);
		startOffset = 4;
		coUint32* rootTableOffset = (coUint32*)pack.buffer.data;
		*rootTableOffset = 4 + vtableSize;
	}
	coResize(pack.buffer, startOffset + vtableSize + tableSize);
	coUint16* newData = (coUint16*)&pack.buffer.data[startOffset];
	*newData = vtableSize;
	++newData;
	*newData = 4;
	++newData;
	for (coUint fieldIdx = 0; fieldIdx < format_.nbFields; ++fieldIdx)
	{
		*newData = 0;
		++newData;
	}
	
	const coUint32 tableOffset = startOffset + vtableSize;
	(coInt32&)*newData = coInt32(startOffset) - coInt32(tableOffset);
	coPackEntry entry(pack, format_, tableOffset);
	return entry;
}

void coPackEntry::SetInlineField(const coConstString& name, const void* data, coUint16 size)
{
	const coUint8 fieldIdx = format.GetIndex(name);
	coASSERT(fieldIdx != coUint8(-1));
	const coInt32 vtableOffset = *((coInt32*)&pack.buffer[tableAbsoluteOffset]);
	coUint16* vtable = (coUint16*)&pack.buffer[tableAbsoluteOffset + vtableOffset];
	const coUint16 offset = format.GetOffset(fieldIdx);
	vtable[1] += size;
	vtable[2 + fieldIdx] = offset;
	coMemCopy(&pack.buffer[tableAbsoluteOffset + offset], data, size);
	//coPushBackArray(pack.buffer, coArray<const coByte>(reinterpret_cast<const coByte*>(&value), sizeof(T)));
}

void coPackEntry::SetField(const coConstString& name, const coConstString& s)
{
	const coUint32 absoluteOffset = pack.buffer.count;
	const coUint32 offset = pack.buffer.count - tableAbsoluteOffset;
	SetInlineField(name, &offset, sizeof(offset));
	coResize(pack.buffer, pack.buffer.count + 4 + s.count + 1);
	coUint32* size = (coUint32*)&pack.buffer.data[absoluteOffset];
	*size = s.count + 1;
	coMemCopy(pack.buffer.data + absoluteOffset + 4, s.data, s.count);
	pack.buffer.data[4 + s.count] = 0;
}

coBool coPackEntry::GetField(const coConstString& name, coDynamicString& value) const
{
	const coUint16 offset = GetFieldOffset(name);
	if (!offset)
		return false;
	const coUint32 valueOffset = *(const coUint32*)(&pack.buffer[tableAbsoluteOffset + offset]);
	const coUint32* data = (const coUint32*)&pack.buffer[tableAbsoluteOffset + valueOffset];
	const coUint32 size = *data - 1;
	coResize(value, size);
	coMemCopy(value.data, data + 1, size);
	return true;
}

coUint16 coPackEntry::GetFieldOffset(const coConstString& name) const
{
	const coUint8 fieldIdx = format.GetIndex(name);
	coASSERT(fieldIdx != coUint8(-1));
	const coInt32 vtableOffset = *((const coInt32*)&pack.buffer[tableAbsoluteOffset]);
	const coUint16* vtable = (const coUint16*)&pack.buffer[tableAbsoluteOffset + vtableOffset];
	return vtable[2 + fieldIdx];
}
