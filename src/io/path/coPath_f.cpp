// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "io/path/coPath_f.h"
#include "io/path/coPathStatus.h"
#include "container/string/coDynamicString_f.h"
#include "container/string/coChar_f.h"
#include "lang/result/coResult_f.h"

void coNormalizePath(coDynamicString& _this)
{
	if (_this.count == 0)
		return;

	const coUint32 len = _this.count;

	coTODO("Use better allocation.");
	coChar* buf = new coChar[len];
	coInt bufIndex = 0;
	coInt lastSlashInBuf = -1;

	for (coUint i = 0; i < len; ++i)
	{
		const coChar c = _this[i];
		switch (c)
		{
		case '/':
		case '\\':
		{
			if (lastSlashInBuf != (bufIndex - 1))
			{
				buf[bufIndex] = '/';
				lastSlashInBuf = bufIndex;
				++bufIndex;
			}
		}
		break;
		case '.':
		case ' ':
		case ':':
		{
			buf[bufIndex] = c;
			++bufIndex;
		}
		break;
		default:
		{
			if (coIsAlphaNumeric(c))
			{
				buf[bufIndex] = c;
				++bufIndex;
			}
		}
		break;
		}
	}

	// remove '/' when last character
	if (bufIndex && buf[bufIndex - 1] == '/')
		--bufIndex;

	coUint startIndex = 0;
	// Remove starting './'
	if (bufIndex > 1 && buf[0] == '.' && buf[1] == '/')
	{
		startIndex = 2;
		bufIndex -= 2;
	}

	_this = coConstString(buf + startIndex, bufIndex);

	delete[] buf;
}

coBool coIsPathNormalized(const coConstString& _this)
{
	coBool lastWasSeparator = false;
	for (const coChar c : _this)
	{
		switch (c)
		{
		case '\\':
			return false;
		case '/':
			if (lastWasSeparator)
				return false;
			lastWasSeparator = true;
			break;
		default:
			lastWasSeparator = false;
		}
	}
	return !lastWasSeparator;
}

void coGetFileName(coConstString& _out, const coConstString& _this)
{
	coASSERT(coIsPathNormalized(_this));
	coUint pos = coFindLastChar(_this, '/');
	pos = (pos != _this.count) ? pos + 1 : 0;
	coGetSubStringFromPos(_out, _this, pos);
}

void coGetBaseName(coConstString& _out, const coConstString& _this)
{
	coASSERT(coIsPathNormalized(_this));
	if (!coIsDotOrDoubleDot(_this))
	{
		coGetFileName(_out, _this);
		const coUint32 pos = coFindLastChar(_out, '.');
		coGetSubStringFromSize(_out, _out, pos);
	}
	else
	{
		_out = coConstString();
	}
}

void coGetExtension(coConstString& _out, const coConstString& _this)
{
	coASSERT(coIsPathNormalized(_this));
	if (!coIsDotOrDoubleDot(_this))
	{
		coConstString fileName;
		coGetFileName(fileName, _this);
		const coUint32 pos = coFindLastChar(fileName, '.');
		coGetSubStringFromPos(_out, fileName, pos);
	}
	else
	{
		_out = coConstString();
	}
}

coBool coIsDot(const coConstString& _this)
{
	return _this.count == 1 && _this.data[0] == '.';
}

coBool coIsDoubleDot(const coConstString& _this)
{
	return _this.count == 2 && _this.data[0] == '.' && _this.data[1] == '.';
}

coBool coIsDotOrDoubleDot(const coConstString& _this)
{
	switch (_this.count)
	{
	case 1: return _this.data[0] == '.';
	case 2: return _this.data[0] == '.' && _this.data[1] == '.';
	default: return false;
	}
}

coBool coIsDotHiddenPath(const coConstString& _this)
{
	return _this.count > 1
		&& (_this.data[0] == '.')
		&& (_this.data[1] != '.')
		&& (coIsFileNameCompatible(_this.data[1]));
}

void coJoinPaths(coDynamicString& _out, const coConstString& _a, const coConstString& _b)
{
	coASSERT(coIsPathNormalized(_a));
	coASSERT(coIsPathNormalized(_b));
	if (_b.count == 0)
	{
		_out = _a;
	}
	else if (_a.count == 0 || coIsDot(_a))
	{
		_out = _b;
	}
	else
	{
		coJoin(_out, _a, _b, '/');
	}
}

void coGetParentDir(coConstString& _out, const coConstString& _this)
{
	coASSERT(coIsPathNormalized(_this));
	const coUint pos = coFindLastChar(_this, '/');
	if (pos == _this.count)
	{
		_out = ".";
	}
	else
	{
		_out.data = _this.data;
		_out.count = pos;
	}
}

coBool coExists(const coConstString& _path)
{
	coASSERT(coIsPathNormalized(_path));
	coPathStatus status;
	coCHECK(coGetPathStatus(status, _path), nullptr);
	return status.Exists();
}

coBool coIsDirectory(const coConstString& _path)
{
	coASSERT(coIsPathNormalized(_path));
	coPathStatus status;
	coCHECK(coGetPathStatus(status, _path), nullptr);
	return status.IsDirectory();
}

coBool coIsFile(const coConstString& _path)
{
	coASSERT(coIsPathNormalized(_path));
	coPathStatus status;
	coCHECK(coGetPathStatus(status, _path), nullptr);
	return status.IsRegularFile();
}
