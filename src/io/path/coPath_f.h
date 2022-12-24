// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
#include <container/string/coDynamicString.h>
class coPathStatus;
class coConstString;
class coDynamicString;

coResult coGetPathStatus(coPathStatus& _status, const coConstString& _path);
coBool coExists(const coConstString& _path);
coBool coIsDirectory(const coConstString& _path);
coBool coIsFile(const coConstString& _path);
void coNormalizePath(coDynamicString& _this);
coBool coIsPathNormalized(const coConstString& _s);
void coGetFileName(coConstString& _out, const coConstString& _this);
void coGetBaseName(coConstString& _out, const coConstString& _this);
void coGetExtension(coConstString& _out, const coConstString& _this);
void coGetParentDir(coConstString& _out, const coConstString& _this);
coResult coGetAbsolutePath(coDynamicString& _out, const coConstString& _this);
coBool coIsDot(const coConstString& _this);
coBool coIsDoubleDot(const coConstString& _this);
coBool coIsDotOrDoubleDot(const coConstString& _this);
coBool coIsDotHiddenPath(const coConstString& _this);
void coJoinPaths(coDynamicString& _out, const coConstString& _a, const coConstString& _b);
coDynamicString coJoinPaths(const coConstString& a, const coConstString& b);
