// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "debug/log/coAssert.h"

class coPackFormat
{
public:
	void AddName(const coChar* name);
	coUint16 GetIndex(coUint32 hash) const;
	const coChar* GetName(coUint16 index);

private:
	coUint16 nbNames = 0;
	coUint32* nameHashes = nullptr;
	coUint16* nameOffsets = nullptr;
	coChar* names = nullptr;
};

coUint16 coPackFormat::GetIndex(coUint32 hash) const
{
	coASSERT(false);
	return 0;
}

const coChar* coPackFormat::GetName(coUint16 index)
{
	coASSERT(index < nbNames);
	const coUint16 offset = nameOffsets[index];
	return &names[offset];
}
