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

	// Replace '\' by '/'
	for (coChar& c : _this)
		if (c == '\\')
			c = '/';

	coRemoveMultiples(_this, '/');

	// remove trailing '/'
	coASSERT(_this.count);
	if (_this.count > 2  && _this.data[_this.count - 1] == '/')
		--_this.count;

	coWARN_NOT_FULLY_FUNCTIONAL();

// 	coUint in = 0;
// 	coUint out = 0;
// 
// 	const coUint countMinus1 = _this.count - 1;
// 	const coUint countMinus2 = _this.count - 2;
// 	coUint lastNameStart = 0;
// 
// 	while (in < _this.count)
// 	{
// 
// 	}
// 	for (; in < _this.count; ++in)
// 	{
// 		const coChar c = _this.data[in];
// 		switch (c)
// 		{
// 		case '/':
// 		{
// 			break;
// 		}
// 		case '.':
// 		{
// 
// 			break;
// 		}
// 		}
// 		while (_this.data[in] == '/')
// 		{
// 			++in;
// 			if (in >= _this.count)
// 				break;
// 		}
// 
// 		_this.data[out] = c;
// 	}
// 
// 	coASSERT(out <= _this.count);
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
	return _this.count == 1 || !lastWasSeparator;
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
