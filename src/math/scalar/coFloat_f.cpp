// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/scalar/coFloat_f.h"

void coSetDenormals(coBool b)
{
	if (b)
	{
		_controlfp_s(NULL, _DN_SAVE, _MCW_DN);
	}
	else
	{
		_controlfp_s(NULL, _DN_FLUSH, _MCW_DN);
	}
}

void coSetFloatingPointExceptions(coBool b)
{
	if (b)
	{
		unsigned int fp_control_state;
		_controlfp_s(&fp_control_state, _EM_INEXACT, _MCW_EM);
	}
	else
	{
		coASSERT(false);
	}
}
