// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/list/coNodeList_f.h"
#include "container/list/coListNode.h"
#include "debug/log/coAssert.h"

void coInsertAfter(coListNode& _node, coListNode& _newNode)
{
	coASSERT(_node.next);
	coInsertBefore(*_node.next, _newNode);
}

void coInsertBefore(coListNode& _node, coListNode& _newNode)
{
	coASSERT(_newNode.previous == &_newNode);
	coASSERT(_newNode.next == &_newNode);
	coListNode* previous = _node.previous;
	coASSERT(previous);
	_node.previous = &_newNode;
	_newNode.next = &_node;
	_newNode.previous = previous;
	previous->next = &_newNode;
}
