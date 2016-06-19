// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/cppExtensions.h"
#include "math/matrix/mat3.h"
#include "math/vector/floatx3_f.h"

namespace co
{
	floatx3 getScale(const mat3& _a)
	{
		return make_floatx3(getLength(_a.c0), getLength(_a.c1), getLength(_a.c2));
	}

	mat3 removeScale(const mat3& _a)
	{
		mat3 m;
		m.c0 = normalize(_a.c0);
		m.c1 = normalize(_a.c1);
		m.c2 = normalize(_a.c2);
		return m;
	}
}
