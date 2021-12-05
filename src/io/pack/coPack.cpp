// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coPack.h"
#include "coPackFormat.h"
#include <container/array/coDynamicArray_f.h>

coPackEntry coPack::PushEntry(const coPackFormat& format_)
{
	coUint32 startOffset = buffer.count;
	const coUint16 vtableSize = 2 * (2 + format_.nbFields);
	const coUint16 tableSize = 4 + format_.totalSize;
	if (startOffset == 0)
	{
		coResize(buffer, 4);
		startOffset = 4;
		coUint32* rootTableOffset = (coUint32*)buffer.data;
		*rootTableOffset = 4 + vtableSize;
	}
	coResize(buffer, startOffset + vtableSize + tableSize);
	coUint16* newData = (coUint16*)&buffer.data[startOffset];
	*newData = vtableSize;
	++newData;
	*newData = format_.GetTotalSize();
	++newData;
	for (coUint fieldIdx = 0; fieldIdx < format_.nbFields; ++fieldIdx)
	{
		*newData = 0;
		++newData;
	}

	const coUint32 tableOffset = startOffset + vtableSize;
	(coInt32&)*newData = -vtableSize;
	coPackEntry entry(*this, format_, tableOffset);
	return entry;
}

coPackEntry coPack::GetRoot(const coPackFormat& format) const
{
    return coPackEntry(const_cast<coPack&>(*this), format, (const coUint32&)buffer[0]);
}
