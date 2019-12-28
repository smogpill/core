// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "debug/log/coAssert.h"
#include "debug/log/coLog.h"

class coResult
{
public:
	coFORCE_INLINE coResult(coBool _success = true)
		: success(_success)
	{
	}

	coFORCE_INLINE ~coResult()
	{
		// This result was not tested, jump back in the call stack to find a way to test the error before it being destroyed.
		coASSERT(tested);
	}

	coFORCE_INLINE operator coBool () const
	{
#ifdef coDEBUG
		tested = true;
#endif
		return success;
	}

	coFORCE_INLINE void operator= (coBool b)
	{
#ifdef coDEBUG
		tested = false;
#endif
		success = b;
	}

	coFORCE_INLINE void operator= (const coResult& b)
	{
#ifdef coDEBUG
		tested = false;
		b.tested = true;
#endif
		success = b;
	}

	coFORCE_INLINE void operator&= (coBool b)
	{
#ifdef coDEBUG
		tested = false;
#endif
		success &= b;
	}

	coFORCE_INLINE void operator&= (const coResult& b)
	{
#ifdef coDEBUG
		tested = false;
		b.tested = true;
#endif
		success &= b;
	}

	coBool success = true;
#ifdef coDEBUG
	mutable coBool tested = false;
#endif
};

