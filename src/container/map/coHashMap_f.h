// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include <container/map/coHashMap.h>
#include <container/map/internal/_coHashMap_f.h>
#include <memory/allocator/coAllocator.h>
#include <lang/coStdWrapper.h>
#include <math/scalar/coUint32_f.h>

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

	coHACK("coHashMap bucket allocation.");
	buckets = static_cast<decltype(buckets)>(allocator->Allocate(1024 * sizeof(*buckets)));
	bucketMask = 1024 - 1;
	coFill(buckets, 1024, _coHashMap_INVALID_INDEX);
}

template <class T>
coHashMap<T>::~coHashMap()
{
	allocator->Free(entries);
	allocator->Free(buckets);
}

template <class T> coFORCE_INLINE coHashMapEntry<T>* coBegin(coHashMap<T>& _this) { return _this.entries; }
template <class T> coFORCE_INLINE const coHashMapEntry<T>* coBegin(const coHashMap<T>& _this) { return _this.entries; }
template <class T> coFORCE_INLINE coHashMapEntry<T>* coEnd(coHashMap<T>& _this) { return _this.entries + _this.count; }
template <class T> coFORCE_INLINE const coHashMapEntry<T>* coEnd(const coHashMap<T>& _this) { return _this.entries + _this.count; }

template <class T>
void coReserve(coHashMap<T>& _this, coUint32 _desiredCapacity)
{
	typedef coHashMapEntry<T> Entry;
	if (_desiredCapacity > _this.capacity)
	{
		const coUint32 roundedCapacity = coNextPowerOf2(_desiredCapacity);
		const coUint32 newCapacity = coMax(16u, roundedCapacity);
		Entry* newEntryBuffer = static_cast<Entry*>(_this.allocator->Allocate(newCapacity * sizeof(T)));
		if (_this.entries)
		{
			coMemCopy(newEntryBuffer, _this.entries, _this.count * sizeof(T));
			_this.allocator->Free(_this.entries);
		}
		_this.capacity = newCapacity;
		_this.entries = newEntryBuffer;
		if (newCapacity > _coGetBucketCount(_this))
		{
			_coRehash(_this, newCapacity);
		}
	}
}

template <class T>
void coClear(coHashMap<T>& _this)
{
	coFill(_this.buckets, _coGetBucketCount(_this), _coHashMap_INVALID_INDEX);
	_this.count = 0;
#ifdef coDEBUG
	coFillAsDeleted(_this.entries, _this.count * sizeof(T));
#endif
}

template <class T>
const T& coGet(const coHashMap<T>& _this, coUint64 _key, const T& _defaultValue)
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

template <class T>
coBool coContains(const coHashMap<T>& _this, coUint64 _key)
{
	return coFind(_this, _key) != nullptr;
}

template <class T>
void coSet(coHashMap<T>& _this, coUint64 _key, const T& _val)
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

template <class T>
coHashMapEntry<T>* coFind(coHashMap<T>& _this, coUint64 _key)
{
	typedef coHashMapEntry<T> Entry;
	const coUint32 bucketIndex = _key & _this.bucketMask;
	coUint32 entryIndex = _this.buckets[bucketIndex];
	while (entryIndex != _coHashMap_INVALID_INDEX)
	{
		Entry& entry = _this.entries[entryIndex];
		if (entry.key == _key)
			return &entry;
		entryIndex = entry.next;
	}
	return coEnd(_this);
}

template <class T>
const coHashMapEntry<T>* coFind(const coHashMap<T>& _this, coUint64 _key)
{
	return const_cast<const coHashMapEntry<T>*>(coFind(const_cast<coHashMap<T>&>(_this), _key));
}

template <class T>
void coRemove(coHashMap<T>& _this, coHashMapEntry<T>* _entry)
{
	_coRemoveEntry(_this, _entry->m_key);
}
