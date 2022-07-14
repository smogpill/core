// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "coResult.h"
#include "debug/log/coLog.h"

#define coTRY(_cond_, _msg_) coSAFE_SCOPE( if(!(_cond_)){ coERROR(_msg_); coBREAK(); return false; } )
#define coCHECK(_cond_, _msg_) coSAFE_SCOPE( if(!(_cond_)){ coERROR(_msg_); coBREAK(); } )
