// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/list/coNodeList_f.h"
#include "container/list/coListNode.h"
#include "debug/log/coAssert.h"

void coInsertAfter(coListNode& _this, coListNode& _newNode)
{
	coASSERT(_newNode.previous == &_newNode);
	coASSERT(_newNode.next == &_newNode);
	coListNode* last = _this.next;
	coASSERT(last);
	_this.next = &_newNode;
	_newNode.previous = &_this;
	_newNode.next = last;
	last->previous = &_newNode;
}
