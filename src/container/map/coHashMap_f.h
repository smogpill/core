// Copyright(c) 2016-2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coHashMapHash_f.h"
#include <container/map/coHashMap.h>
#include <container/map/internal/_coHashMap_f.h>
#include <lang/coStdWrapper.h>
#include <math/scalar/coUint32_f.h>

template <class K, class T, coUint NB_BUCKETS, class Hash> coFORCE_INLINE coHashMapEntry<K, T>* coBegin(coHashMap<K, T, NB_BUCKETS, Hash>& _this) { return _this.entries; }
template <class K, class T, coUint NB_BUCKETS, class Hash> coFORCE_INLINE const coHashMapEntry<K, T>* coBegin(const coHashMap<K, T, NB_BUCKETS, Hash>& _this) { return _this.entries; }
template <class K, class T, coUint NB_BUCKETS, class Hash> coFORCE_INLINE coHashMapEntry<K, T>* coEnd(coHashMap<K, T, NB_BUCKETS, Hash>& _this) { return _this.entries + _this.count; }
template <class K, class T, coUint NB_BUCKETS, class Hash> coFORCE_INLINE const coHashMapEntry<K, T>* coEnd(const coHashMap<K, T, NB_BUCKETS, Hash>& _this) { return _this.entries + _this.count; }

template <class K, class T, coUint NB_BUCKETS, class Hash>
void coReserve(coHashMap<K, T, NB_BUCKETS, Hash>& _this, coUint32 _desiredCapacity)
{
	typedef coHashMapEntry<K, T> Entry;
	if (_desiredCapacity > _this.capacity)
	{
		const coUint32 roundedCapacity = coNextPowerOf2(_desiredCapacity);
		const coUint32 newCapacity = coMax(16u, roundedCapacity);
		Entry* newEntryBuffer = static_cast<Entry*>(_this.allocator->Allocate(newCapacity * sizeof(Entry)));
		if (_this.entries)
		{
			coMemCopy(newEntryBuffer, _this.entries, _this.count * sizeof(Entry));
			_this.allocator->Free(_this.entries);
		}
		_this.capacity = newCapacity;
		_this.entries = newEntryBuffer;
	}
}

template <class K, class T, coUint NB_BUCKETS, class Hash>
void coClear(coHashMap<K, T, NB_BUCKETS, Hash>& _this)
{
	coFill(_this.buckets, NB_BUCKETS, coHashMap<K, T, NB_BUCKETS, Hash>::invalidIndex);
	_this.count = 0;
#ifdef coDEBUG
	coFillAsDeleted(_this.entries, _this.count * sizeof(coHashMapEntry<K, T>));
#endif
}

template <class K, class T, coUint NB_BUCKETS, class Hash>
const T& coGet(const coHashMap<K, T, NB_BUCKETS, Hash>& _this, const K& _key, const T& _defaultValue)
{
	typedef coHashMapEntry<K, T> Entry;
	const coHashMapEntry<K, T>* entry = coFind(_this, _key);
	if (entry)
	{
		return entry->value;
	}
	else
	{
		return _defaultValue;
	}
}

template <class K, class T, coUint NB_BUCKETS, class Hash>
coBool coContains(const coHashMap<K, T, NB_BUCKETS, Hash>& _this, const K& _key)
{
	return coFind(_this, _key) != nullptr;
}

template <class K, class T, coUint NB_BUCKETS, class Hash>
void coSet(coHashMap<K, T, NB_BUCKETS, Hash>& _this, const K& _key, const T& _val)
{
	typedef coHashMapEntry<K, T> Entry;
	Entry* entry = coFind(_this, _key);
	if (entry)
	{
		entry->value = _val;
	}
	else
	{
		_coAddEntry(_this, _key, _val);
	}
}

template <class K, class T, coUint NB_BUCKETS, class Hash>
coHashMapEntry<K, T>* coFind(coHashMap<K, T, NB_BUCKETS, Hash>& _this, const K& _key)
{
	typedef coHashMapEntry<K, T> Entry;
	const coUint32 bucketIndex = Hash()(_key) & coHashMap<K, T, NB_BUCKETS, Hash>::bucketMask;
	coUint32 entryIndex = _this.buckets[bucketIndex];
	while (entryIndex != coHashMap<K, T, NB_BUCKETS>::invalidIndex)
	{
		Entry& entry = _this.entries[entryIndex];
		if (entry.key == _key)
			return &entry;
		entryIndex = entry.next;
	}
	return nullptr;
}

template <class K, class T, coUint NB_BUCKETS, class Hash>
const coHashMapEntry<K, T>* coFind(const coHashMap<K, T, NB_BUCKETS, Hash>& _this, const K& _key)
{
	return const_cast<const coHashMapEntry<K, T>*>(coFind(const_cast<coHashMap<K, T, NB_BUCKETS, Hash>&>(_this), _key));
}

template <class K, class T, coUint NB_BUCKETS, class Hash>
void coRemove(coHashMap<K, T, NB_BUCKETS, Hash>& _this, coHashMapEntry<K, T>* _entry)
{
	_coRemoveEntry(_this, _entry->key);
}

template <class K, class T, coUint NB_BUCKETS, class Hash>
void coRemove(coHashMap<K, T, NB_BUCKETS, Hash>& _this, const K& _key)
{
	_coRemoveEntry(_this, _key);
}
