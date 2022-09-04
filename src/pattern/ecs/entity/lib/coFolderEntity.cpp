// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coFolderEntity.h"
#include "../coEntityTypeDecl_f.h"
#include "../../component/ownership/coOwnership.h"

coDEFINE_ENTITY_TYPE(coFolderEntity)
{
	type->AddComponent<coOwnership>();
}
