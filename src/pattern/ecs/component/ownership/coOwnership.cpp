// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "pattern/pch.h"
#include "coOwnership.h"
#include "../coComponent_f.h"
#include "../../entity/coEntityData.h"
#include <io/archive/coArchive.h>

coUint32 coWriteOwnership(coArchive& archive, const void* obj)
{
	const coEntityDataContext* context = static_cast<const coEntityDataContext*>(archive.GetContext());
	coEntityWorld* world = context->world;
	const coOwnership* ownership = static_cast<const coOwnership*>(obj);
	auto& data = archive.GetData();
	const coUint32 index = data.count;
	archive.PushBytes(2 * sizeof(coUint32));
	const coUint32 nextSiblingIdx = coEntityData::Write(archive, ownership->nextSibling);
	const coUint32 firstChildIdx = coEntityData::Write(archive, ownership->firstChild);
	coUint32* inlineData = reinterpret_cast<coUint32*>(&data[index]);
	inlineData[0] = nextSiblingIdx;
	inlineData[1] = firstChildIdx;
	return index;
}

void coReadOwnership(const coArchive& archive, coUint32 idx, void* obj)
{
	const coEntityDataContext* context = static_cast<const coEntityDataContext*>(archive.GetContext());
	coEntityWorld* world = context->world;
	coOwnership* ownership = static_cast<coOwnership*>(obj);
	const auto& data = archive.GetData();
	const coUint32* inlineData = reinterpret_cast<const coUint32*>(&data[idx]);
	const coUint32 nextSiblingIdx = inlineData[0];
	const coUint32 firstChildIdx = inlineData[1];
	if (nextSiblingIdx)
		ownership->nextSibling = coEntityData::Read(archive, nextSiblingIdx);
	if (firstChildIdx)
		ownership->firstChild = coEntityData::Read(archive, firstChildIdx);
}

coDEFINE_COMPONENT(coOwnership)
{
	type->writeArchiveFunc = &coWriteOwnership;
	type->readArchiveFunc = &coReadOwnership;
}
