// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "debug/log.h"

namespace co
{
	struct ListNode
	{
		ListNode()
		{
			previous = this;
			next = this;
		}
		~ListNode()
		{
			coASSERT(previous == this);
			coASSERT(next == this);
			previous->next = next;
			next->previous = previous;
		}
		ListNode* previous;
		ListNode* next;
	};

	template <class T>
	struct ListNodeData : public ListNode
	{
		typedef T DataType;
		ListNodeData(const T& _data) : data(_data) {}
		T data;
	};

	template <class T>
	class List
	{
		friend struct coList_f;
	public:
		typedef T DataType;
		List() : size(0)
		{
		}
		~List()
		{
			ListNode* node = endNode.next;
			while (node != &endNode)
			{
				ListNode* next = node->next;
				node->previous = node;
				node->next = node;
				node = next;
			}

			// To avoid assert raised on node's destructor
			endNode.previous = &endNode;
			endNode.next = &endNode;
		}
	protected:
		ListNode	endNode;
		uint32		size;
	};
}
