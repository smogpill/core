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
