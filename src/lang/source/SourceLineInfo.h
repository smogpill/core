// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

namespace co
{
	class SourceLineInfo
	{
	public:
		SourceLineInfo() : file(nullptr), line(0) {}
		const char*	file;
		int			line;
	};
}
