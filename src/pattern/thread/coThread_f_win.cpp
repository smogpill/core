// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "pattern/thread/coThread_f.h"

coUint64 GetGetCurrentThreadID()
{
	return static_cast<coUint64>(::GetCurrentThreadId());
}
