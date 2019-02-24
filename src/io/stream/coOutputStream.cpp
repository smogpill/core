// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/stream/coOutputStream.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

coOutputStream::~coOutputStream()
{
	for (auto* p : blocks)
		delete[] p;
}

void coOutputStream::Write(coByte value)
{
	if (coUNLIKELY(posInBlock == s_blockSize))
	{
		++blockIndex;
		if (blockIndex == blocks.count)
		{
			coPushBack(blocks, new coByte[s_blockSize]);
		}
		posInBlock = 0;
	}
	coByte* block = blocks[blockIndex];
	block[posInBlock++] = value;
}

void coOutputStream::Write(const void* data, coUint size)
{
	do
	{
		const coUint available = s_blockSize - posInBlock;
		if (coLIKELY(available >= size))
		{
			coByte* block = blocks[blockIndex];
			coMemCopy(&block[posInBlock], data, size);
			posInBlock += size;
			return;
		}
		else
		{
			if (available)
			{
				coByte* block = blocks[blockIndex];
				coMemCopy(&block[posInBlock], data, available);
				size -= available;
			}
			++blockIndex;
			if (blockIndex == blocks.count)
			{
				coPushBack(blocks, new coByte[s_blockSize]);
			}
			posInBlock = 0;
			reinterpret_cast<const coByte*&>(data) += available;
		}
	} while (true);
}

void coOutputStream::SetPos(coUint32 pos)
{
	blockIndex = pos / s_blockSize;
	posInBlock = pos % s_blockSize;
	if (coUNLIKELY(blockIndex >= blocks.count))
	{
		const coUint nbBlocks = blocks.count;
		coResize(blocks, blockIndex + 1);
		for (coUint i = nbBlocks; i < blocks.count; ++i)
		{
			blocks[i] = new coByte[s_blockSize];
		}
	}
}

void coOutputStream::GetOutput(coDynamicArray<coByte>& output) const
{
	const coUint size = GetPos();
	coResize(output, size);
	if (size == 0)
		return;
	coUint pos = 0;
	for (coUint i = 0; i < blocks.count - 1; ++i)
	{
		const coByte* block = blocks[i];
		coMemCopy(&output.data[pos], block, s_blockSize);
		pos += s_blockSize;
	}
	coMemCopy(&output.data[pos], coBack(blocks), posInBlock);
}
