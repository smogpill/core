// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "detail/coMutexBase.h"

class coRecursiveMutex final : public _coMutexBase<std::recursive_mutex>
{
public:
	coRecursiveMutex() {}
	coRecursiveMutex(const coRecursiveMutex&) = delete;
	coRecursiveMutex& operator=(const coRecursiveMutex&) = delete;
};
