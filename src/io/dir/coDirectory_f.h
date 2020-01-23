// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

#include "lang/result/coResult.h"

class coConstString;

coResult coCreateDir(const coConstString& _path);
coResult coCreateDirsIfMissing(const coConstString& _rawPath);
coResult coDeleteDir(const coConstString& path);