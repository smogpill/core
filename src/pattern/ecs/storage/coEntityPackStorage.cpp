// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coEntityPackStorage.h"
#include <lang/reflect/coTypeDecl_f.h>
#include <io/archive/coArchive.h>
#include "../coECS.h"

coDEFINE_CLASS(coEntityPackStorage)
{
	coDEFINE_FIELD(entities);
}
