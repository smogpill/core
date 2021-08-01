// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/vector/coInt32x3_f.h"
#include <container/string/coDynamicString_f.h>

coDynamicString& operator<<(coDynamicString& s, const coInt32x3& a)
{
	s << "[" << a.x << ", " << a.y << ", " << a.z << "]";
	return s;
}