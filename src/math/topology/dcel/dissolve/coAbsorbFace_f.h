// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coDCEL;

/// Returns an arbitrary half edge of the resulting face or coUint32(-1) if no joining took place
coUint32 coAbsorbTwinFace(coDCEL& dcel, coUint32 edgeIdx);
