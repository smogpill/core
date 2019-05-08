// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coLock
{
public:
	coLock();
	~coLock();

	void Lock();
	coBool TryLock();
	void Unlock();
	std::aligned_storage<40, 8>::type impl;
};
