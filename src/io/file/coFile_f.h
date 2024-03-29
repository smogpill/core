// Copyright(c) 2016-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
class coConstString;

coResult coGetFileSize(coUint64& _size8, const coConstString& _path);
coResult coGetFileTimes(coUint64& _creationTime, coUint64& _lastAccessTime, coUint64& _lastWriteTime, const coConstString& _path);
coResult coReadFullFile(coDynamicArray<coByte>& _out, const coConstString& _path);
/// Creates the directories and the file if necessary
coResult coEasyWriteToFile(const coConstString& path, const coConstString& content);
coResult coMoveFile(const coConstString& from, const coConstString& to);
coResult coDeleteFile(const coConstString& path);
coBool coIsWritable(const coConstString& path);
coResult coDumpTGA(const coConstString& path, coUint width, coUint height, const coArray<coByte>& buffer);
