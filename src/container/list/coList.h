// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coListNode
{
public:
	coListNode();
	~coListNode();
	coListNode* previous;
	coListNode* next;
};

template <class T>
class coListNodeData : public coListNode
{
public:
	typedef T DataType;
	coListNodeData(const T& _data) : data(_data) {}
	T data;
};

template <class T>
class coList
{
public:
	typedef T DataType;
	coList() : size(0)
	{
	}
	~coList()
	{
		coListNode* node = endNode.next;
		while (node != &endNode)
		{
			coListNode* next = node->next;
			node->previous = node;
			node->next = node;
			node = next;
		}

		// To avoid assert raised on node's destructor
		endNode.previous = &endNode;
		endNode.next = &endNode;
	}
	coListNode endNode;
	coUint32 size;
};
