// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/map/coHashMap.h"

class _coHashMapFindResult
{
public:
	coUint32 bucket;
	coUint32 previousEntry;
	coUint32 entry;
};

template <class T>
_coHashMapFindResult _coFindExt(const coHashMap<T>& _this, coUint64 _key)
{
	typedef coHashMapEntry<T> Entry;

	_coHashMapFindResult result;
	if (_this.count == 0)
	{
		result.bucket = coHashMap<T>::invalidIndex;
		result.entry = coHashMap<T>::invalidIndex;
		result.previousEntry = coHashMap<T>::invalidIndex;
		return result;
	}

	result.bucket = _key & _this.bucketMask;
	result.entry = _this.buckets[result.bucket];
	result.previousEntry = coHashMap<T>::invalidIndex;
	while (result.entry != coHashMap<T>::invalidIndex)
	{
		const Entry* entry = _this.entries[result.entry];
		if (entry->key == _key)
			break;
		result.previousEntry = result.entry;
		result.entry = entry->next;
	}

	return result;
}

template <class T>
coHashMapEntry<T>* _coAddEntry(coHashMap<T>& _this, coUint64 _key, const T& _val)
{
	coReserve(_this, _this.count + 1);
	typedef coHashMapEntry<T> Entry;
	const coUint32 bucketIndex = _key & _this.bucketMask;
	coUint32& bucket = _this.buckets[bucketIndex];
	Entry& entry = _this.entries[_this.count];
	entry.key = _key;
	entry.next = bucket;
	entry.value = _val;
	bucket = _this.count;
	++_this.count;
	return &entry;
}
template <class T>
void _coRemoveEntry(coHashMap<T>& _this, coUint64 _key)
{
	typedef coHashMapEntry<T> Entry;

	const _coHashMapFindResult result = _coFindExt(_this, _key);
	if (result.entry == coHashMap<T>::invalidIndex)
		return;

	Entry& entry = _this.entries[result.entry];

	if (result.previousEntry == coHashMap<T>::invalidIndex)
		_this.buckets[result.bucket] = entry.next;
	else
		_this.entries[result.previousEntry].next = entry.next;

	if (result.entry != _this.count - 1)
	{
		Entry& entry = _this.entries[_this.count - 1];
		_coHashMapFindResult last = _coFindExt(_this, entry.key);
		if (last.previousEntry == coHashMap<T>::invalidIndex)
			_this.buckets[last.bucket].next = result.entry;
		else
			_this.entries[last.previousEntry].next = result.entry;
	}

	--_this.count;
}

template <class T>
coFORCE_INLINE coUint32 _coGetBucketCount(const coHashMap<T>& _this)
{
	return _this.bucketMask + 1;
}
