// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "detail/coMutexBase.h"

class coMutex final : public _coMutexBase<std::mutex>
{
public:
	coMutex() = default;
	coMutex(const coMutex&) = delete;
	coMutex& operator=(const coMutex&) = delete;
};
