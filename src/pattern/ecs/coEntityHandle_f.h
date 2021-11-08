// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coEntityHandle.h"

coFORCE_INLINE coBool coIsNull(const coEntityHandle& h) { return h.val == 0; }