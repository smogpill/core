// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "container/list/coNodeList.h"
#include "container/list/coListNode.h"

template <class T>
class coList
{
public:
	typedef T DataType;
	struct Entry : public coListNode
	{
		T data;
	};

	coList(){}
	coList(std::initializer_list<T> _l);

	coListNode endNode;
};
