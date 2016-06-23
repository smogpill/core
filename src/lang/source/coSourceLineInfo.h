// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

class coSourceLineInfo
{
public:
	coSourceLineInfo() : file(nullptr), line(0) {}
	const char*	file;
	int			line;
};
