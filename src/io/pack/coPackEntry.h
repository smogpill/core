// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coPackFormat.h"
class coPack;

class coPackEntry
{
public:
	coPackEntry(coPack& pack, const coPackFormat& format, coUint32 offset = 0) : pack(pack), format(format), tableAbsoluteOffset(offset) {}
	coPackEntry PushEntry(const coPackFormat& format);
	template <class T>
	void SetField(coPackEntry& entry, const coConstString& name, const T& value);
	
	coPack& pack;
	const coPackFormat& format;
	coUint32 vtableAbsoluteOffset = 0;
	coUint32 tableAbsoluteOffset = 0;
};

template <class T>
inline void coPackEntry::SetField(coPackEntry& entry, const coConstString& name, const T& value)
{
	const coUint8 fieldIdx = entry.format.GetIndex(name);
	coASSERT(fieldIdx != coUint8(-1));
	coUint16* vtable = (coUint16*)&entry.pack.buffer[entry.vtableAbsoluteOffset];
	vtable[1] += sizeof(T);
	vtable[2 + fieldIdx] = entry.format.GetOffset(fieldIdx);
	coPushBackArray(entry.pack.buffer, coArray<const coByte>(reinterpret_cast<const coByte*>(&value), sizeof(T)));
}
