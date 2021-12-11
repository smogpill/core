// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coClock.h"
#include <platform/coOs.h>
#include <math/scalar/coFloat_f.h>

coClock::coClock()
{
	rootTimeStamp = coGetTimeStamp();
	lastTimeStamp = rootTimeStamp;
}

void coClock::Step()
{
	const coUint64 timeStamp = coGetTimeStamp();
	if (fixedDt >= 0.0f)
	{
		rawDt = fixedDt;
		dt = fixedDt;
		time += fixedDt;
	}
	else
	{
		const coFloat64 newTime = coGetSeconds(timeStamp - rootTimeStamp);
		rawDt = coFloat(newTime - time);
		dt = coMin(rawDt, maxDt);
		time = newTime;
	}
	lastTimeStamp = timeStamp;
	++nbSteps;
}
