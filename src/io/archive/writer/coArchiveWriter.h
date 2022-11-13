// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/dynamic/coReverseDynamicArray.h>

class coType;

class coArchiveWriter
{
public:
	void Clear();
	void WriteRoot(const void* object, const coType& type);
	template <class T>
	void WriteRoot(const T& object);

private:
	void Write(const void* buffer, coUint32 size);
	template <class T>
	void Write(const T& buffer) { WriteBuffer(&buffer, sizeof(buffer)); }
	void PushBytes(coUint size);

	coReverseDynamicArray<coByte> data;
};

template<class T>
inline void coArchiveWriter::WriteRoot(const T& object)
{
	WriteRoot(&object, *T::GetStaticType());
}
