// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coPackFormat.h"
class coPack;
class coConstString;
class coDynamicString;

class coPackEntry
{
public:
	coPackEntry(coPack& pack, const coPackFormat& format, coUint32 offset = 0) : pack(pack), format(format), tableAbsoluteOffset(offset) {}
	coPackEntry PushEntry(const coPackFormat& format);
	template <class T>
	void SetField(const coConstString& name, const T& value) { SetInlineField(name, &value, sizeof(T)); }
	void SetField(const coConstString& name, const coConstString& s);
	void SetInlineField(const coConstString& name, const void* data, coUint16 size);
	template <class T>
	coBool GetField(const coConstString& name, T& value) const;
	coBool GetField(const coConstString& name, coDynamicString& value) const;
	coBool GetInlineField(const coConstString& name, void* data, coUint16 size) const;
	
private:
	coUint16 GetFieldOffset(const coConstString& name) const;
	coPack& pack;
	const coPackFormat& format;
	coUint32 tableAbsoluteOffset = 0;
};

template <class T>
inline coBool coPackEntry::GetField(const coConstString& name, T& value) const
{
	const coUint16 offset = GetFieldOffset(name);
	if (!offset)
		return false;
	coUint8* data = &pack.buffer[tableAbsoluteOffset + offset];
	value = *reinterpret_cast<T*>(data);
	return true;
}
