// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coThread.h"

coUint64 coGetGetCurrentThreadID();
void coSleep(coUint milliseconds);
coUint coGetMaxConcurrentThreads();
