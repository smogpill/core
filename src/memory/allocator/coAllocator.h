// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coAllocator
{
public:
	virtual ~coAllocator() {}

	virtual void* allocate(coUint32 _size8) = 0;
	virtual void* allocateAligned(coUint32 _size8, coUint _alignment) = 0;
	virtual void free(void* _p) = 0;
	virtual void freeAligned(void* _p) = 0;
	static coAllocator* getHeap();

private:
	static void init();

	static coAllocator*	heap;
	static coAllocator*	frame;
	static bool initialized;
};
