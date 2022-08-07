// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coComponentTypeHandle.h"
#include <debug/log/coAssert.h>

const coComponentTypeHandle coComponentTypeHandle::invalid;

coComponentTypeHandle::coComponentTypeHandle(const coUint index_)
	: index(coUint16(index_))
{
	coASSERT(index_ <= (coUint)coUint16(-1));
}
