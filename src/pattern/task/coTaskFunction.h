// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

enum class coTaskResult
{
	DONE,
	YIELD,
};

using coTaskFunction = std::function<coTaskResult()>;