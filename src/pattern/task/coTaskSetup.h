// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

enum class coTaskPriority
{
	HIGH,
	DEFAULT,
	LOW,

	END
};

class coTaskSetup
{
public:
	void SetPriority(coTaskPriority priority) { _priority = priority; }
private:
	friend class coTask;
	coTaskPriority _priority = coTaskPriority::DEFAULT;
};