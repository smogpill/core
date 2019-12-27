// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/scalar/coFloat_f.h"
#include <debug/log/coLog.h>

void coSetDenormals(coBool b)
{
	const errno_t err = _controlfp_s(nullptr, b ? _DN_SAVE : _DN_FLUSH, _MCW_DN);
	if (err)
	{
		coERROR("coSetDenormals(" << b << "). Error: " << err);
	}
}

void coSetFloatingPointExceptions(coBool b)
{
	const errno_t err = _controlfp_s(nullptr, b ? 0 : _MCW_EM, _MCW_EM); // 1 hides the exception, 0 sets it.
	if (err)
	{
		coERROR("coSetFloatingPointExceptions(" << b << "). Error: " << err);
	}
}
