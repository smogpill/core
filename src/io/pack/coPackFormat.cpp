// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coPackFormat.h"
#include "math/hash/coHash_f.h"

void coPackFormat::AddField(const coConstString& name, coUint8 index)
{
	Reserve(nbFields + 1);
	nameHashes[nbFields] = coHash32(name);
	indices[nbFields] = index;
	++nbFields;
}

coUint8 coPackFormat::GetIndex(const coConstString& name) const
{
	const coUint32 hash = coHash32(name);
	for (coUint i = 0; i < nbFields; ++i)
	{
		if (nameHashes[i] == hash)
		{
			return indices[i];
		}
	}
	return coUint8(-1);
}

const coChar* coPackFormat::GetName(coUint8 index)
{
	coASSERT(index < nbFields);
	const coUint16 offset = nameOffsets[index];
	return &names[offset];
}

void coPackFormat::Reserve(coUint16 nb)
{
	if (capacity < nb)
	{
		coASSERT(nbFields <= capacity);
		auto* newNameHashes = new coUint32[nb];
		auto* newIndices = new coUint8[nb];
		coMemCopy(newNameHashes, nameHashes, nbFields * sizeof(*nameHashes));
		coMemCopy(newIndices, indices, nbFields * sizeof(*indices));
		delete[] nameHashes;
		delete[] indices;
		nameHashes = newNameHashes;
		indices = newIndices;
		capacity = nb;
	}
}
