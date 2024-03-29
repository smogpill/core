// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "math/pch.h"
#include "math/vector/coVec3.h"
#include <lang/reflect/coTypeDecl_f.h>

coBEGIN_CLASS(coVec3)
type->serializableSize8 = sizeof(coFloat) * 3;
coEND_CLASS()
