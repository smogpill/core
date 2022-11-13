// Copyright(c) 2019 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coTextArchiveWriter.h"
#include "../coArchive_f.h"
#include "../coArchiveFormat.h"
#include "lang/result/coResult_f.h"
#include "container/string/coDynamicString_f.h"

coTextArchiveWriter::coTextArchiveWriter(const coArchive& pack_, const coArchiveFormat& format_)
	: pack(pack_)
	, format(format_)
{

}

coResult coTextArchiveWriter::Write(coDynamicString& text_)
{
	text = &text_;
	coTRY(WriteBlock(), nullptr);
	return true;
}

coResult coTextArchiveWriter::WriteBlock()
{
	*text << "{\n";
	*text << "}\n";
	return true;
}
