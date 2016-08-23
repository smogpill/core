// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/file/coFile_f.h"
#include "io/path/coPath_f.h"
#include "io/path/coPathStatus.h"
#include "io/file/coFileStreamBuffer.h"
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

	coFileStreamBuffer streamBuffer;
	{
		coFileStreamBuffer::InitConfig c;
		c.path = _path;
		c.mode = coFileStreamBuffer::read;
		coTRY(streamBuffer.Init(c), "Failed to open for writing: " << streamBuffer);
	}

	coResize(_out, size8);
	const coUint sizeRead = streamBuffer.Read(_out.data, size8);
	coTRY(streamBuffer.GetResult(), "Failed to read: " << _path);
	coTRY(sizeRead == size8, _path << ": requested read of " << size8 << " bytes, but only " << sizeRead << " bytes were read.");
	return true;
}
