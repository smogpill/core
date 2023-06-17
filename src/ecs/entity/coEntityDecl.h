// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

enum class coEntityState : coUint8
{
	NIL,
	CREATED,
	READY,
	STARTED,

	TARGET_STATE_END = STARTED,

	UNREADYING,
	READYING,

	STOPPING,
	STARTING,

	END
};
