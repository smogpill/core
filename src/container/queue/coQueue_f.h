// Copyright(c) 2020 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coQueue.h"
#include <debug/log/coAssert.h>

template <class T>
coBool coIsEmpty(const coQueue<T>& q)
{
	return q.count == 0;
}
