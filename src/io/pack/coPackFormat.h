// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "debug/log/coAssert.h"

class coConstString;

class coPackFormat
{
public:
	static const coUint8 s_invalidFieldIndex = coUint8(-1);
	void AddField(const coConstString& name, coUint8 index);
	coUint8 GetIndex(const coConstString& name) const;
	const coChar* GetName(coUint8 index);

private:
	coUint16 nbFields = 0;
	coUint32* nameHashes = nullptr;
	coUint8* indices = nullptr;
	coUint16* nameOffsets = nullptr;
	coChar* names = nullptr;
};

inline coUint8 coPackFormat::GetIndex(const coConstString& name) const
{
	coASSERT(false);
	return 0;
}

inline const coChar* coPackFormat::GetName(coUint8 index)
{
	coASSERT(index < nbFields);
	const coUint16 offset = nameOffsets[index];
	return &names[offset];
}
