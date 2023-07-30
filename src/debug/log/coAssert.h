// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCppExtensions.h"

#ifdef coDEBUG
#	define coASSERT(_cond_) coSAFE_SCOPE( if(!(_cond_) && coReturnTrue()) [[unlikely]] { coBREAK(); } )
#else
#	define coASSERT(_cond_)
#endif
