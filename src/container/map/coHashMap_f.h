// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include <container/map/coHashMap.h>
#include <container/map/internal/_coHashMap_f.h>
#include <lang/coStdWrapper.h>
#include <math/scalar/coUint32_f.h>

template <class T, coUint NB_BUCKETS> coFORCE_INLINE coHashMapEntry<T>* coBegin(coHashMap<T, NB_BUCKETS>& _this) { return _this.entries; }
template <class T, coUint NB_BUCKETS> coFORCE_INLINE const coHashMapEntry<T>* coBegin(const coHashMap<T, NB_BUCKETS>& _this) { return _this.entries; }
template <class T, coUint NB_BUCKETS> coFORCE_INLINE coHashMapEntry<T>* coEnd(coHashMap<T, NB_BUCKETS>& _this) { return _this.entries + _this.count; }
template <class T, coUint NB_BUCKETS> coFORCE_INLINE const coHashMapEntry<T>* coEnd(const coHashMap<T, NB_BUCKETS>& _this) { return _this.entries + _this.count; }

template <class T, coUint NB_BUCKETS>
void coReserve(coHashMap<T, NB_BUCKETS>& _this, coUint32 _desiredCapacity)
{
	typedef coHashMapEntry<T> Entry;
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

template <class T, coUint NB_BUCKETS>
void coClear(coHashMap<T, NB_BUCKETS>& _this)
{
	coFill(_this.buckets, NB_BUCKETS, coHashMap<T, NB_BUCKETS>::invalidIndex);
	_this.count = 0;
#ifdef coDEBUG
	coFillAsDeleted(_this.entries, _this.count * sizeof(coHashMapEntry<T>));
#endif
}

template <class T, coUint NB_BUCKETS>
const T& coGet(const coHashMap<T, NB_BUCKETS>& _this, coUint64 _key, const T& _defaultValue)
{
	typedef coHashMapEntry<T> Entry;
	const coHashMapEntry<T>* entry = coFind(_this, _key);
	if (entry)
	{
		return entry->value;
	}
	else
	{
		return _defaultValue;
	}
}

template <class T, coUint NB_BUCKETS>
coBool coContains(const coHashMap<T, NB_BUCKETS>& _this, coUint64 _key)
{
	return coFind(_this, _key) != nullptr;
}

template <class T, coUint NB_BUCKETS>
void coSet(coHashMap<T, NB_BUCKETS>& _this, coUint64 _key, const T& _val)
{
	typedef coHashMapEntry<T> Entry;
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

template <class T, coUint NB_BUCKETS>
coHashMapEntry<T>* coFind(coHashMap<T, NB_BUCKETS>& _this, coUint64 _key)
{
	typedef coHashMapEntry<T> Entry;
	const coUint32 bucketIndex = _key & coHashMap<T, NB_BUCKETS>::bucketMask;
	coUint32 entryIndex = _this.buckets[bucketIndex];
	while (entryIndex != coHashMap<T, NB_BUCKETS>::invalidIndex)
	{
		Entry& entry = _this.entries[entryIndex];
		if (entry.key == _key)
			return &entry;
		entryIndex = entry.next;
	}
	return nullptr;
}

template <class T, coUint NB_BUCKETS>
const coHashMapEntry<T>* coFind(const coHashMap<T, NB_BUCKETS>& _this, coUint64 _key)
{
	return const_cast<const coHashMapEntry<T>*>(coFind(const_cast<coHashMap<T, NB_BUCKETS>&>(_this), _key));
}

template <class T, coUint NB_BUCKETS>
void coRemove(coHashMap<T, NB_BUCKETS>& _this, coHashMapEntry<T>* _entry)
{
	_coRemoveEntry(_this, _entry->key);
}
