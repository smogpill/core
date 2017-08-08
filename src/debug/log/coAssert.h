// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/coCompiler.h"
#include "lang/coCppExtensions.h"

#ifdef coDEBUG
#	define coASSERT(_cond_) coSAFE_SCOPE( if(coUNLIKELY(!(_cond_) && coReturnTrue())){ coBREAK(); } )
#else
#	define coASSERT(_cond_)
#endif
