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
	void SetField(const coConstString& name, const T& value);
	template <class T>
	coBool GetField(const coConstString& name, T& value) const;
	
	coPack& pack;
	const coPackFormat& format;
	coUint32 tableAbsoluteOffset = 0;
};

template <class T>
inline void coPackEntry::SetField(const coConstString& name, const T& value)
{
	const coUint8 fieldIdx = format.GetIndex(name);
	coASSERT(fieldIdx != coUint8(-1));
	const coInt32 vtableOffset = (coInt32&)pack.buffer[tableAbsoluteOffset];
	coUint16* vtable = (coUint16*)&pack.buffer[tableAbsoluteOffset + vtableOffset];
	const coUint16 offset = format.GetOffset(fieldIdx);
	vtable[1] += sizeof(T);
	vtable[2 + fieldIdx] = offset;
	T* target = (T*)(&pack.buffer[tableAbsoluteOffset + offset]);
	*target = value;
	//coPushBackArray(pack.buffer, coArray<const coByte>(reinterpret_cast<const coByte*>(&value), sizeof(T)));
}

template <class T>
inline coBool coPackEntry::GetField(const coConstString& name, T& value) const
{
	const coUint8 fieldIdx = format.GetIndex(name);
	coASSERT(fieldIdx != coUint8(-1));
	const coInt32 vtableOffset = *((const coInt32*)&pack.buffer[tableAbsoluteOffset]);
	const coUint16* vtable = (const coUint16*)&pack.buffer[tableAbsoluteOffset + vtableOffset];
	const coUint16 offset = vtable[2 + fieldIdx];
	if (!offset)
		return false;
	coUint8* data = &pack.buffer[tableAbsoluteOffset + offset];
	value = *reinterpret_cast<T*>(data);
	return true;
}
