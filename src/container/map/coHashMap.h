// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

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
