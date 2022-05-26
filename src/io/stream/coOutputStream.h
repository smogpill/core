// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <lang/reflect/coTypeDecl.h>
#include <io/stream/coStream.h>

class coOutputStream : public coStream
{
	coDECLARE_SUPER(coStream);
public:
	~coOutputStream();
	void Write(coByte value);
	void Write(const void* data, coUint size);
	void SetPos(coUint32 pos);
	coUint32 GetPos() const { return blockIndex * s_blockSize + posInBlock; }
	void GetOutput(coDynamicArray<coByte>& output) const;

private:
	void Flush();
	static const coUint s_blockSize = 4096;
	coDynamicArray<coByte*> blocks;
	coUint blockIndex = coUint(-1);
	coUint posInBlock = s_blockSize;
};
