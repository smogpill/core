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
	void Reserve(coUint16 nb);

	coUint16 nbFields = 0;
	coUint16 capacity = 0;
	coUint32* nameHashes = nullptr;
	coUint8* indices = nullptr;
	coUint16* nameOffsets = nullptr;
	coChar* names = nullptr;
};
