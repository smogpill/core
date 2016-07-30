// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "prebuild/pch.h"
#include "prebuild/generator/plugins/coCppGeneratorUtils.h"
#include "io/stream/coStringOutputStream.h"
#include "container/string/coConstString.h"
#include "io/path/coPath_f.h"

void coWriteInclude(coStringOutputStream& _stream, const coConstString& _path)
{
	coASSERT(coIsPathNormalized(_path));
	_stream << "#include \"" << _path << "\"\n";
}

void coWriteHeader(coStringOutputStream& _stream)
{
	_stream << "// Copyright(c) 2016 Jounayd Id Salah\n";
	_stream << "// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).\n";
	_stream << "// GENERATED FILE\n";
}
