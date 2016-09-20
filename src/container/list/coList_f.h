// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/list/coList.h"
#include "container/list/coNodeList_f.h"
#include "lang/coStdWrapper.h"

template <class T>
class coListIterator
{
public:
	typedef typename  coList<T>::Entry Entry;
	typedef coListIterator<T> Iterator;

	coListIterator(typename Entry* _entry) : entry(_entry) {}
	const Entry& operator*() const { return *entry; }
	Entry& operator*() { return *entry; }
	void operator++()
	{
		entry = static_cast<Entry*>(entry->next);
	}
	coBool operator!=(const Iterator& _other) const { return entry != _other.entry; }
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
	const Entry& operator*() const { return *entry; }
	void operator++()
	{
		entry = static_cast<const Entry*>(entry->next);
	}
	coBool operator!=(const Iterator& _other) const { return entry != _other.entry; }
private:
	const Entry* entry;
};

template <class T>
coList<T>::coList(std::initializer_list<T> _l)
{
	for (const T& v : _l)
		coPushBack(*this, v);
}

template <class T> coFORCE_INLINE coListConstIterator<T> coBegin(const coList<T>& _this) { return coListConstIterator<T>(static_cast<const coList<T>::Entry*>(_this.endNode.next)); }
template <class T> coFORCE_INLINE coListIterator<T> coBegin(coList<T>& _this) { return coListIterator<T>(static_cast<coList<T>::Entry*>(_this.endNode.next)); }
template <class T> coFORCE_INLINE coListConstIterator<T> coEnd(const coList<T>& _this) { return coListConstIterator<T>(static_cast<const coList<T>::Entry*>(&_this.endNode)); }
template <class T> coFORCE_INLINE coListIterator<T> coEnd(coList<T>& _this) { return coListIterator<T>(static_cast<coList<T>::Entry*>(&_this.endNode)); }
template <class T> coFORCE_INLINE void coPushBack(coList<T>& _this, const T& _v)
{
	coInsertBefore(static_cast<coList<T>::Entry&>(_this.endNode), _v);
}
template <class T> coFORCE_INLINE void coInsertBefore(typename coList<T>::Entry& _at, const T& _v)
{
	coList<T>::Entry* entry = new coList<T>::Entry();
	entry->data = _v;
	coInsertBefore(_at, *entry);
}
template <class T> coFORCE_INLINE void coInsertAfter(typename coList<T>::Entry& _at, const T& _v)
{
	coInsertBefore(static_cast<coList<T>::Entry&>(*_at.next), _v);
}
template <class T> coFORCE_INLINE coBool coIsEmpty(const coList<T>& _this) { return _this.endNode.next == &_this.endNode; }
template <class T> typename coList<T>::Entry* coFind(coList<T>& _this, const T& _val)
{
	for (auto& e : _this)
	{
		if (e.data == _val)
		{
			return &e;
		}
	}
	return nullptr;
}
