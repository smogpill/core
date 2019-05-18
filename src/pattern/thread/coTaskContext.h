// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coTaskScheduler;

class coTaskContext
{
public:
	coTaskScheduler* scheduler = nullptr;
	coUint workerIndex = 0;
};
