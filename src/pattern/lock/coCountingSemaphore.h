// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <debug/log/coAssert.h>

// Implementation based on the Microsoft's STL implementation
// std::counting_semaphore was not used because:
// - No flexibility on the size of the internal atomics
// - No access to the internal counter to retrieve its value
// - No Acquire(N)

template <class T>
class _coCountingSemaphore
{
public:
	constexpr explicit _coCountingSemaphore(const T nbDesired = 0) : _counter(nbDesired) {}
	_coCountingSemaphore(const _coCountingSemaphore&) = delete;
	_coCountingSemaphore& operator=(const _coCountingSemaphore&) = delete;

	static constexpr T(Max)() { return std::numeric_limits<T>::max(); }

	void Acquire()
	{
		// Copied from Microsoft's STL impl

		T current = _counter.load(std::memory_order_relaxed);
		for (;;)
		{
			while (current == 0)
			{
				Wait(_Atomic_wait_no_timeout);
				current = _counter.load(std::memory_order_relaxed);
			}
			coASSERT(current > 0 && current <= Max());

			// "happens after release" ordering is provided by this CAS, so loads and waits can be relaxed
			if (_counter.compare_exchange_weak(current, current - 1))
				return;
		}
	}
	void Acquire(T nb) { while (nb--) Acquire(); }
	coBool TryAcquire()
	{
		// Copied from Microsoft's STL impl

		T current = _counter.load();
		if (current == 0)
			return false;
		coASSERT(current > 0 && current <= Max());

		return _counter.compare_exchange_weak(current, current - 1);
	}
	void Release(T nb = 1)
	{
		// Copied from Microsoft's STL impl

		if (nb == 0)
			return;

		const T prev = _counter.fetch_add(nb);
		coASSERT(prev + nb > 0 && prev + nb <= Max());

		const T waitingUpperBound = _waiting.load();

		if (waitingUpperBound == 0)
		{
			// Definitely no one is waiting
		}
		else if (waitingUpperBound <= nb)
		{
			// No more waiting threads than update, can wake everyone.
			_counter.notify_all();
		}
		else
		{
			// Wake at most _Update. Though repeated notify_one() is somewhat less efficient than single notify_all(),
			// the amount of OS calls is still the same; the benefit from trying not to wake unnecessary threads
			// is expected to be greater than the loss on extra calls and atomic operations.
			for (; nb != 0; --nb)
				_counter.notify_one();
		}
	}
	T GetValue() const { return _counter; }
private:

	void Wait(coUint32 remainingTimeout) 
	{
		// Copied from Microsoft's STL impl
		
		// See the comment in Release()
		_waiting.fetch_add(1);
		T current = _counter.load();

		// TODO: Not standard, should find an appropriate implementation when porting.
		// std::atomic::wait() seems to do a bit more than necessary.
		if (current == 0)
			__std_atomic_wait_direct(&_counter, &current, sizeof(current), remainingTimeout);

		_waiting.fetch_sub(1, std::memory_order_relaxed);
	}

	std::atomic<T> _counter;
	std::atomic<T> _waiting;
};

using coCountingSemaphore32 = _coCountingSemaphore<coInt32>;