// Copyright(c) 2019-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coArchive;
class coArchiveFormat;

class coArchiveObject
{
public:
	coArchive* archive = nullptr;
	const coArchiveFormat* format = nullptr;
	coUint32 offset = 0;
};
