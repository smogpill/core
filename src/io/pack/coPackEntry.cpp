// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coPackEntry.h"
#include "coPack.h"
#include <container/array/coDynamicArray_f.h>

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
