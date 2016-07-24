// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coAllocator
{
public:
	virtual ~coAllocator() {}

	virtual void* Allocate(coUint32 _size8) = 0;
	virtual void* AllocateAligned(coUint32 _size8, coUint _alignment) = 0;
	virtual void Free(void* _p) = 0;
	virtual void FreeAligned(void* _p) = 0;
	static coAllocator* GetHeap();
	static coAllocator* GetDebug();
	static coAllocator* GetFrame();
	static coAllocator* GetStack();

private:
	static void Init();

	static coAllocator*	heap;
	static coAllocator*	frame;
	static coAllocator* debug;
	static coAllocator* stack;
	static bool initialized;
};
