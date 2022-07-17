// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coArchiveClass
{
public:
	coUint8 nbFields = 0;
	coUint8* fieldIDs = nullptr;
	coUint32* fieldHashes = nullptr;
	coUint16* fieldOffsets = nullptr;
	coChar* fieldNames = nullptr;
};
