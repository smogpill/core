// Copyright(c) 2023 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <container/array/coDynamicArray_f.h>

template <class ...Args> class coSlot;

template <class ...Args>
class coSignal
{
	using Slot = coSlot<Args...>;
public:
	~coSignal()
	{
		for (Slot* slot : slots)
			slot->_OnDisconnected()
	}
	void Connect(Slot& slot)
	{
		coPushBack(slots, &slot);
	}
	void Disconnect(Slot& slot)
	{
		const coUint32 index = coFind(slots, &slot);
		if (index < slots.count)
		{
			slot._OnDisconnected();
			coRemoveUnorderedByIndex(slots, index);
		}
	}
	void Disconnect()
	{
		for (Slot* slot : slots)
			slot->_OnDisconnected()
		coClear(slots);
	}
	void Emit(Args... params)
	{
		for (Slot* slot : slots)
			slot->Call(params);
	}

	void _OnDisconnected(Slot& s)
	{
		coRemoveUnordered(slots, &s);
	}
	
private:
	coDynamicArray<Slot*> slots;
};

template <class ...Args>
class coSlot
{
	using Function = std::function<void(Args...)>;
	using Signal = coSignal<Args...>;
public:
	coSlot() = default;
	coSlot(Function&& f) : function(f) {}
	~coSlot()
	{
		Disconnect();
	}

	void Call(Args... params)
	{
		if (function)
			function(params);
	}

	void Disconnect()
	{
		for (Signal* signal : signals)
			signal->_OnDisconnected(*this);
		coClear(signals);
	}

	void _OnConnected(Signal& s)
	{
		coASSERT(!coContains(signals, s));
		coPushBack(signals, &s);
	}

	void _OnDisconnected(Signal& s)
	{
		coRemoveUnordered(signals, &s);
	}

private:
	Function function;
	coDynamicArray<Signal*> signals;
};
