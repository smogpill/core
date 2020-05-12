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
struct coHashMapHash
{
	coUint32 operator()(const T& v) const;
};

template <>
struct coHashMapHash<coUint64>
{
	coUint32 operator()(const coUint64& v) const;
};

template <class T, coUint NB_BUCKETS, class Hash = coHashMapHash<coUint64>>
class coHashMap
{
public:
	enum : coUint32
	{
		invalidIndex = static_cast<coUint32>(-1),
		bucketMask = NB_BUCKETS - 1
	};
	coHashMap();
	coHashMap(coAllocator& _allocator);
	~coHashMap();

	typedef coHashMapEntry<T> Entry;

	Entry* entries = nullptr;
	coUint32* buckets = nullptr;
	coUint32 count = 0;
	coUint32 capacity = 0;
	coAllocator* allocator = nullptr;
};

template <class T, coUint NB_BUCKETS, class Hash>
coHashMap<T, NB_BUCKETS, Hash>::coHashMap()
	: coHashMap(*coAllocator::GetHeap())
{

}

template <class T, coUint NB_BUCKETS, class Hash>
coHashMap<T, NB_BUCKETS, Hash>::coHashMap(coAllocator& _allocator)
	: allocator(&_allocator)
{
	static_assert(std::is_trivially_copyable<T>::value, "Trivially copyable only");

	//coHACK("coHashMap bucket allocation.");
	buckets = static_cast<decltype(buckets)>(allocator->Allocate(NB_BUCKETS * sizeof(*buckets)));
	coFill(buckets, NB_BUCKETS, invalidIndex);
}

template <class T, coUint NB_BUCKETS, class Hash>
coHashMap<T, NB_BUCKETS, Hash>::~coHashMap()
{
	allocator->Free(entries);
	allocator->Free(buckets);
}