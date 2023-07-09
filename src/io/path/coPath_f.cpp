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

	if (coStartsWith(_this, "./"))
	{
		if (_this.count == 2)
		{
			_this.count = 1;
		}
		else
		{
			_this = coGetSubStringFromPos(_this, 2);
		}
	}

	// remove trailing '/'
	coASSERT(_this.count);
	if (_this.count > 2  && _this.data[_this.count - 1] == '/')
		--_this.count;

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

coConstString coGetFileName(const coConstString& _this)
{
	coASSERT(coIsPathNormalized(_this));
	coUint pos = coFindLastChar(_this, '/');
	pos = (pos != _this.count) ? pos + 1 : 0;
	return coGetSubStringFromPos(_this, pos);
}

coConstString coGetBaseName(const coConstString& _this)
{
	coASSERT(coIsPathNormalized(_this));

	coConstString out;
	if (!coIsDotOrDoubleDot(_this))
	{
		out = coGetFileName(_this);
		const coUint32 pos = coFindLastChar(out, '.');
		out = coGetSubStringFromSize(out, pos);
	}

	return out;
}

coConstString coGetExtension(const coConstString& _this)
{
	coConstString out;
	coASSERT(coIsPathNormalized(_this));
	if (!coIsDotOrDoubleDot(_this))
	{
		const coConstString fileName = coGetFileName(_this);
		const coUint32 pos = coFindLastChar(fileName, '.');
		out = coGetSubStringFromPos(fileName, pos);
	}
	return out;
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

coDynamicString coJoinPaths(const coConstString& a, const coConstString& b)
{
	coDynamicString out;
	coJoinPaths(out, a, b);
	return out;
}

coDynamicString coJoinPaths3(const coConstString& a, const coConstString& b, const coConstString& c)
{
	coDynamicString out;
	coJoinPaths(out, coJoinPaths(a, b), c);
	return out;
}

coConstString coGetParentDir(const coConstString& _this)
{
	coASSERT(coIsPathNormalized(_this));
	const coUint pos = coFindLastChar(_this, '/');
	if (pos == _this.count)
	{
		return coConstString(".");
	}
	else
	{
		return coConstString(_this.data, pos);
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
