// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "container/list/coList.h"
#include "container/list/coNodeList_f.h"

template <class T>
class coListIterator
{
public:
	coListIterator(coListNodeData<T>* _node) : node(_node) {}
	T& operator*() const { return node->data; }
	T& operator*() { return node->data; }
	void operator++()
	{
		node = static_cast<coListNodeData<T>*>(node->next);
	}
	coBool operator!=(const coListIterator<T>& _other) const { return node == _other.node; }
private:
	coListNodeData<T>* node;
};

template <class T>
class coListConstIterator
{
public:
	coListConstIterator(const coListNodeData<T>* _node) : node(_node) {}
	const T& operator*() const { return node->data; }
	T& operator*() { return node->data; }
	void operator++()
	{
		node = static_cast<const coListNodeData<T>*>(node->next);
	}
	coBool operator!=(const coListIterator<T>& _other) const { return node == _other.node; }
private:
	const coListNodeData<T>* node;
};

template <class T> coFORCE_INLINE coListConstIterator<T> coBegin(const coList<T>& _this) { return coListConstIterator<T>(static_cast<const coListNodeData<T>*>(_this.endNode.next)); }
template <class T> coFORCE_INLINE coListIterator<T> coBegin(coList<T>& _this) { return coListIterator<T>(static_cast<coListNodeData<T>*>(_this.endNode.next)); }
template <class T> coFORCE_INLINE coListConstIterator<T> coEnd(const coList<T>& _this) { return coListConstIterator<T>(static_cast<const coListNodeData<T>*>(&_this.endNode)); }
template <class T> coFORCE_INLINE coListIterator<T> coEnd(coList<T>& _this) { return coListIterator<T>(static_cast<coListNodeData<T>*>(&_this.endNode)); }
