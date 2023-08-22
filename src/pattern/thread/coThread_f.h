// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coThread.h"

enum class coThreadPriority
{
	IDLE,
	LOW,
	NORMAL,
	HIGH
};

void coSleep(coUint milliseconds);
coUint coGetMaxConcurrentThreads();
void coSetThreadName(const coChar* name);
void coSetThreadPriority(coThreadPriority priority);
