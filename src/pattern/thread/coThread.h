// Copyright(c) 2019-2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coThreadSetup
{
	const coChar* name = nullptr;
};

class coThread
{
public:
	template<class Function>
	explicit coThread(const coThreadSetup& setup, Function&& f)
	{
		auto threadMain = []()
		{
			f();
		};
		_impl = std::move(std::thread(threadMain));
	}
	coThread(const coThread&) = delete;
	~coThread();

	coUint64 GetID() const;

	void Stop();
	void RequestStop() { stopRequested = true; }
	coBool IsStopRequested() const { return stopRequested; }

private:
	template <class Function, class... Args>
	void ThreadMain(Function&& f, Args&&... args)
	{

		f(args);
	}
	std::thread _impl;
	
	volatile coBool stopRequested = false;
};
