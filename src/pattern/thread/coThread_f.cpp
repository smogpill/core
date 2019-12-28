// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coThread_f.h"

void coSleep(coUint milliseconds)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

coUint coGetMaxConcurrentThreads()
{
	return std::thread::hardware_concurrency();
}
