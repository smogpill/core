// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

constexpr inline coUint16 co_maxNbComponentsPerWorld = 1024;
static_assert(co_maxNbComponentsPerWorld % 64 == 0);

constexpr inline coUint8 co_maxNbComponentsPerProcessor = 32;

using coEntityPatternID = coUint32;

class coComponentMaskHandle
{
public:
private:
	coUint32 index = coUint32(-1);
};