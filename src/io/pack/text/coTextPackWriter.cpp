// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coTextPackWriter.h"
#include "../coPack_f.h"
#include "../coPackFormat.h"
#include "lang/result/coResult_f.h"
#include "container/string/coDynamicString_f.h"

coTextPackWriter::coTextPackWriter(const coPack& pack_, const coPackFormat& format_)
	: pack(pack_)
	, format(format_)
{

}

coResult coTextPackWriter::Write(coDynamicString& text_)
{
	text = &text_;
	coTRY(WriteBlock(), nullptr);
	return true;
}

coResult coTextPackWriter::WriteBlock()
{
	*text << "{\n";
	*text << "}\n";
	return true;
}
