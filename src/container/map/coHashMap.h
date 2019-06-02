// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <memory/allocator/coAllocator.h>
#include <debug/log/coLog.h>

class coAllocator;

template <class T>
struct coHashMapEntry
{
	coUint64 key;
	coUint32 next;
	T value;
};

template <class T>
class coHashMap
{
public:
	enum : coUint32
	{
		invalidIndex = static_cast<coUint32>(-1)
	};
	coHashMap();
	coHashMap(coAllocator& _allocator);
	~coHashMap();

	typedef coHashMapEntry<T> Entry;

	Entry* entries = nullptr;
	coUint32* buckets = nullptr;
	coUint32 bucketMask = 0;
	coUint32 count = 0;
	coUint32 capacity = 0;
	coAllocator* allocator = nullptr;
};

template <class T>
coHashMap<T>::coHashMap()
	: coHashMap(*coAllocator::GetHeap())
{

}

template <class T>
coHashMap<T>::coHashMap(coAllocator& _allocator)
	: allocator(&_allocator)
{
	static_assert(std::is_trivially_copyable<T>::value, "Trivially copyable only");

	//coHACK("coHashMap bucket allocation.");
	buckets = static_cast<decltype(buckets)>(allocator->Allocate(1024 * sizeof(*buckets)));
	bucketMask = 1024 - 1;
	coFill(buckets, 1024, invalidIndex);
}

template <class T>
coHashMap<T>::~coHashMap()
{
	allocator->Free(entries);
	allocator->Free(buckets);
}