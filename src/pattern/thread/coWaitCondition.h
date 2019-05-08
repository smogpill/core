// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coWaitCondition
{
public:
	coWaitCondition();
	~coWaitCondition();
	void WakeOne();
	void Wait();
	coBool Wait(coUint milliseconds);

private:
	coUint64 platformSpecific = 0;
};
