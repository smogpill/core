// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/list/coList.h"
#include "container/list/coNodeList_f.h"

template <class T>
class coListIterator
{
public:
	typedef typename  coList<T>::Entry Entry;
	typedef coListIterator<T> Iterator;

	coListIterator(typename Entry* _entry) : entry(_entry) {}
	T& operator*() const { return entry->data; }
	T& operator*() { return entry->data; }
	void operator++()
	{
		entry = static_cast<Entry*>(entry->next);
	}
	coBool operator!=(const Iterator& _other) const { return entry == _other.entry; }
private:
	Entry* entry;
};

template <class T>
class coListConstIterator
{
	typedef typename coList<T>::Entry Entry;
	typedef coListConstIterator<T> Iterator;
public:
	coListConstIterator(const Entry* _entry) : entry(_entry) {}
	const T& operator*() const { return entry->data; }
	T& operator*() { return entry->data; }
	void operator++()
	{
		entry = static_cast<const Entry*>(entry->next);
	}
	coBool operator!=(const Iterator& _other) const { return entry == _other.entry; }
private:
	const Entry* entry;
};

template <class T> coFORCE_INLINE coListConstIterator<T> coBegin(const coList<T>& _this) { return coListConstIterator<T>(static_cast<const coList<T>::Entry*>(_this.endNode.next)); }
template <class T> coFORCE_INLINE coListIterator<T> coBegin(coList<T>& _this) { return coListIterator<T>(static_cast<coList<T>::Entry*>(_this.endNode.next)); }
template <class T> coFORCE_INLINE coListConstIterator<T> coEnd(const coList<T>& _this) { return coListConstIterator<T>(static_cast<const coList<T>::Entry*>(&_this.endNode)); }
template <class T> coFORCE_INLINE coListIterator<T> coEnd(coList<T>& _this) { return coListIterator<T>(static_cast<coList<T>::Entry*>(&_this.endNode)); }
