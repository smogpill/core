// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/file/coFile_f.h"
#include "io/path/coPath_f.h"
#include "io/path/coPathStatus.h"
#include "io/file/coFileAccess.h"
#include "lang/result/coResult_f.h"
#include "lang/reflect/coNumericLimits.h"

coResult coGetFileSize(coUint64& _size8, const coConstString& _path)
{
	_size8 = 0;
	coFileAccess access;
	coFileAccess::InitConfig c;
	c.mode = coFileAccess::read;
	c.path = _path;
	coTRY(access.Init(c), "Failed to init file access: " << _path);
	coTRY(access.GetSize8(_size8), "Failed to get the size: "<<_path);
	return true;
}

coResult coReadFullFile(coDynamicArray<coByte>& _out, const coConstString& _path)
{
	coTRY(coIsPathNormalized(_path), "The path is not normalized: " << _path);

	coUint64 largeSize8;
	coTRY(coGetFileSize(largeSize8, _path), "Failed to get the file size: " << _path);

	coTRY(largeSize8 < static_cast<coUint64>(coNumericLimits<coUint32>::Max()), "File size of " << _path << "too big to be read: " << largeSize8 << " bytes");

	coUint32 size8;
	coNumericConvert(size8, largeSize8);

	coFileAccess file;
	{
		coFileAccess::InitConfig c;
		c.mode = coFileAccess::read;
		c.path = _path;
		coTRY(file.Init(c), "Failed to init for reading: " << file);
	}

	coClear(_out);
	coResize(_out, size8);
	coTRY(file.Read(_out), "Failed to read " << file);
	return true;
}

coResult coMoveFile(const coConstString& from, const coConstString& to)
{
	coDynamicString from2(from);
	coNullTerminate(from2);
	coDynamicString to2(to);
	coNullTerminate(to2);
	coTRY(std::rename(from2.data, to2.data) == 0, "Failed to move "<<from<<" to "<<to);
	return true;
}

coResult coDumpTGA(const coConstString& path, coUint16 width, coUint16 height, const coArray<coByte>& buffer)
{
	coDynamicArray<coByte> reorderedBuffer;
	coPushBackArray(reorderedBuffer, buffer);

	// RGB => BGR
	for (coUint i = 0; i < reorderedBuffer.count; i += 3)
		coSwap(reorderedBuffer[i + 0], reorderedBuffer[i + 2]);

	struct TGAHeader
	{
		coByte IDLength = 0; // No info
		coByte colorMapType = 0; // None
		coByte imageType = 2; // Uncompressed true-color image
		coByte colorMapSpec[5] = { 0 };
		coUint16 originX = 0;
		coUint16 originY = 0;
		coUint16 width = 0;
		coUint16 height = 0;
		coUint8 bitsPerPixel = 24;
		coUint8 imageDescriptor = 0;
	};

	TGAHeader header;
	header.width = width;
	header.height = height;

	{
		coFileAccess file;
		coFileAccess::InitConfig c;
		c.mode = coFileAccess::write;
		c.path = path;
		coTRY(file.Init(c), nullptr);
		coTRY(file.Write(coArray<coByte>(reinterpret_cast<coByte*>(&header), sizeof(header))), nullptr);
		coTRY(file.Write(reorderedBuffer), nullptr);
	}
	return true;
}
