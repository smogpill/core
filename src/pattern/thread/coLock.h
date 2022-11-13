// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "pattern/pimpl/coPimpl.h"

class coLock
{
public:
	coLock();
	~coLock();

	void Lock();
	coBool TryLock();
	void Unlock();

private:
	coDECLARE_PIMPL(40, 8);
};

class coLockScope
{
public:
	coLockScope(coLock& lock_) : lock(lock_)
	{
		lock.Lock();
	}
	~coLockScope()
	{
		lock.Unlock();
	}
private:
	coLock& lock;
};
