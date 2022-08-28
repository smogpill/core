// Copyright(c) 2019-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray.h>

class coType;
class coField;

class coArchive
{
public:
	void Clear();
	void WriteRoot(const void* object, const coType& type);
	template <class T>
	void WriteRoot(const T& object);
	void* CreateObjects(const coType& expectedRootBaseType) const;
	template <class T>
	T* CreateObjects() const { return static_cast<T*>(CreateObjects(*T::GetStaticType())); }
	void* CreateObjects(coUint32 idx, const coType& expectedBaseType) const;
	template <class T>
	T* CreateObjects(coUint32 idx) const { return static_cast<T*>(CreateObjects(idx, *T::GetStaticType())); }
	void ReadObject(coUint32 idx, void* object, const coType& type) const;
	template <class T>
	void ReadObject(coUint32 idx, T& object) const { ReadObject(idx, &object, *T::GetStaticType()); }
	void Write(const void* buffer, coUint32 size);
	template <class T>
	void Write(const T& buffer) { Write(&buffer, sizeof(T)); }
	void Read(void* buffer, coUint32 size) const;
	template <class T>
	void Read(T& buffer) const { Read(&buffer, sizeof(buffer)); }
	void SetContext(void* context_) const { context = context_; }
	
	coUint32 WriteObject(const void* object, const coType& type);
	void PushBytes(coUint size);
	void ReadBuffer(coUint32 idx, void*, coUint32 size) const;

	template <class T>
	const T& Get(coUint32 idx) const;
	coDynamicArray<coByte>& GetData() { return data; }
	const coDynamicArray<coByte>& GetData() const { return data; }
	coUint32 GetSize() const { return data.count; }
	coUint32 GetRoot() const;
	void* GetContext() const { return context; }

private:
	static coBool IsFieldInlinable(const coField& field);

	coDynamicArray<coByte> data;
	mutable void* context = nullptr;
};

template<class T>
inline void coArchive::WriteRoot(const T& object)
{
	WriteRoot(&object, *T::GetStaticType());
}

template <class T>
inline const T& coArchive::Get(coUint32 idx) const
{
	coASSERT(idx + sizeof(T) <= data.count);
	return *reinterpret_cast<const T*>(&data.data[idx]);
}