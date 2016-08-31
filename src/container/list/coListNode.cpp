// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "container/pch.h"
#include "container/list/coListNode.h"
#include "debug/log/coAssert.h"

coListNode::coListNode()
{
	previous = this;
	next = this;
}

coListNode::~coListNode()
{
	coASSERT(previous == this);
	coASSERT(next == this);
	previous->next = next;
	next->previous = previous;
}
