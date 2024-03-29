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

template <class K, class T, coUint NB_BUCKETS, class Hash>
_coHashMapFindResult _coFindExt(const coHashMap<K, T, NB_BUCKETS, Hash>& _this, const K& _key)
{
	typedef coHashMapEntry<K, T> Entry;
	typedef coHashMap<K, T, NB_BUCKETS, Hash> Self;

	_coHashMapFindResult result;
	if (_this.count == 0)
	{
		result.bucket = Self::invalidIndex;
		result.entry = Self::invalidIndex;
		result.previousEntry = Self::invalidIndex;
		return result;
	}

	result.bucket = Hash()(_key) & Self::bucketMask;
	result.entry = _this.buckets[result.bucket];
	result.previousEntry = Self::invalidIndex;
	while (result.entry != Self::invalidIndex)
	{
		const Entry& entry = _this.entries[result.entry];
		if (entry.key == _key)
			break;
		result.previousEntry = result.entry;
		result.entry = entry.next;
	}

	return result;
}

template <class K, class T, coUint NB_BUCKETS, class Hash>
coHashMapEntry<K, T>* _coAddEntry(coHashMap<K, T, NB_BUCKETS, Hash>& _this, const K& _key, const T& _val)
{
	typedef coHashMap<K, T, NB_BUCKETS, Hash> Self;
	typedef coHashMapEntry<K, T> Entry;
	coReserve(_this, _this.count + 1);
	const coUint32 bucketIndex = Hash()(_key) & Self::bucketMask;
	coUint32& bucket = _this.buckets[bucketIndex];
	Entry& entry = _this.entries[_this.count];
	entry.key = _key;
	entry.next = bucket;
	entry.value = _val;
	bucket = _this.count;
	++_this.count;
	return &entry;
}
template <class K, class T, coUint NB_BUCKETS, class Hash>
void _coRemoveEntry(coHashMap<K, T, NB_BUCKETS, Hash>& _this, const K& _key)
{
	typedef coHashMapEntry<K, T> Entry;
	typedef coHashMap<K, T, NB_BUCKETS, Hash> Self;

	const _coHashMapFindResult result = _coFindExt(_this, _key);
	if (result.entry == Self::invalidIndex)
		return;

	Entry& entry = _this.entries[result.entry];

	if (result.previousEntry == Self::invalidIndex)
		_this.buckets[result.bucket] = entry.next;
	else
		_this.entries[result.previousEntry].next = entry.next;

	if (result.entry != _this.count - 1)
	{
		Entry& lastEntry = _this.entries[_this.count - 1];
		_coHashMapFindResult last = _coFindExt(_this, lastEntry.key);
		if (last.previousEntry == Self::invalidIndex)
			_this.buckets[last.bucket] = result.entry;
		else
			_this.entries[last.previousEntry].next = result.entry;
	}

	--_this.count;
}
