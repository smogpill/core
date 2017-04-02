// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/transform/coTransform_f.h"
#include "debug/log/coLog.h"

coBool32x4 coIsValid(const coTransform& _this)
{
	return coIsValid(_this.rotation)
		&& coBool32x4(coIsValid(_this.translation), true)
		&& coBool32x4(coIsValid(_this.scale), true);
}
