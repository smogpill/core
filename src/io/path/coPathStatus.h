// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coPathStatus
{
public:
	enum class Status
	{
		none = 0,
		notFound,
		error,

		regularFile,
		directory,
		socket,
		reparseFile
	};

	coPathStatus() : status(Status::none) {}

	coBool IsRegularFile() const { return status == Status::regularFile; }
	coBool IsDirectory() const { return status == Status::directory; }
	coBool Exists() const { return status >= Status::regularFile; }

	Status status;
};
