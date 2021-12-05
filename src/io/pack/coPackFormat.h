// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "debug/log/coAssert.h"

class coConstString;

class coPackFormat
{
public:
	static const coUint8 s_invalidFieldIndex = coUint8(-1);
	void AddField(const coConstString& name, coUint16 size8);
	coUint8 GetIndex(const coConstString& name) const;
	const coChar* GetName(coUint8 index) const;
	coUint16 GetOffset(coUint8 index) const;
	coUint16 GetTotalSize() const { return totalSize; }

	static const coPackFormat empty;

private:
	friend class coPackEntry;
	friend class coPack;
	void Reserve(coUint16 nb);

	coUint16 nbFields = 0;
	coUint16 capacity = 0;
	coUint32* nameHashes = nullptr;
	coUint8* ids = nullptr;
	coUint16* offsets = nullptr;
	coUint16* nameOffsets = nullptr;
	coChar* names = nullptr;
	coUint16 totalSize = 4;
};
