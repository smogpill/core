// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "debug/log/coAssert.h"

class coResult
{
public:
	coFORCE_INLINE coResult(coBool _success = true)
		: success(_success)
#ifdef coDEBUG
		, tested(false)
#endif
	{
	}

	coFORCE_INLINE ~coResult()
	{
		// This result was not tested, jump back in the call stack to find a way to test the error before it being destroyed
		coASSERT(tested);
	}

	coFORCE_INLINE operator coBool () const
	{
#ifdef coDEBUG
		tested = true;
#endif
		return success;
	}

	coFORCE_INLINE void operator= (coBool _b)
	{
#ifdef coDEBUG
		tested = false;
#endif
		success = _b;
	}

	coBool success;
#ifdef coDEBUG
	mutable coBool tested;
#endif
};

#define coTRY(_cond_, _msg_) coSAFE_SCOPE( if(!(_cond_)){ coERROR(_msg_); return false; } )
#define coCHECK(_cond_, _msg_) coSAFE_SCOPE( if(!(_cond_)){ coERROR(_msg_); } )
