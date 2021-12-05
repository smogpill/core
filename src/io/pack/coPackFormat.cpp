// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coPackFormat.h"
#include "math/hash/coHash_f.h"

const coPackFormat coPackFormat::empty;

void coPackFormat::AddField(const coConstString& name, coUint16 size8)
{
	Reserve(nbFields + 1);
	nameHashes[nbFields] = coHash32(name);
	offsets[nbFields] = totalSize;
	totalSize += size8;
	++nbFields;
}

coUint8 coPackFormat::GetIndex(const coConstString& name) const
{
	const coUint32 hash = coHash32(name);
	for (coUint i = 0; i < nbFields; ++i)
	{
		if (nameHashes[i] == hash)
		{
			return coUint8(i);
		}
	}
	return coUint8(-1);
}

const coChar* coPackFormat::GetName(coUint8 index) const
{
	coASSERT(index < nbFields);
	const coUint16 offset = nameOffsets[index];
	return &names[offset];
}

coUint16 coPackFormat::GetOffset(coUint8 index) const
{
	coASSERT(index < nbFields);
	return offsets[index];
}

void coPackFormat::Reserve(coUint16 nb)
{
	if (capacity < nb)
	{
		coASSERT(nbFields <= capacity);
		auto* newNameHashes = new coUint32[nb];
		auto* newIndices = new coUint8[nb];
		auto* newOffsets = new coUint16[nb];
		coMemCopy(newNameHashes, nameHashes, nbFields * sizeof(*nameHashes));
		coMemCopy(newIndices, ids, nbFields * sizeof(*ids));
		coMemCopy(newOffsets, offsets, nbFields * sizeof(*offsets));
		delete[] nameHashes;
		delete[] ids;
		delete[] offsets;
		nameHashes = newNameHashes;
		ids = newIndices;
		offsets = newOffsets;
		capacity = nb;
	}
}
