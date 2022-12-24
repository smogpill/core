// Copyright(c) 2019-2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#include "io/pch.h"
#include "coDescReader.h"
#include <lang/result/coResult_f.h>
#include <io/stream/coStringInputStream.h>
#include <container/string/coChar_f.h>

coResult coDescReader::Read(coStringInputStream& stream, void* object, const coType& type)
{
	coTRY(object, nullptr);
	line = 0;
	if (!ReadObject(stream, object, type))
	{
		coERROR("Failed to parse stream: " << stream.GetDebugName() << "(" << line << ")");
		return false;
	}
	return true;
}

coResult coDescReader::ReadObject(coStringInputStream& stream, void* object, const coType& type)
{
	PassWhitespace(stream);
	coChar c = stream.ReadChar();
	coTRY(c == '{', "Invalid object");
	do
	{
		PassWhitespace(stream);
		coTRY(!stream.IsEnd(), "Premature end of stream");
		c = stream.GetChar();
		if (c == '}')
		{
			break;
		}
		else
		{
			coTRY(ReadExpression(stream, object, type), nullptr);
		}
	} while (true);
	return true;
}

coResult coDescReader::ReadExpression(coStringInputStream& stream, void* object, const coType& type)
{
	if (object == nullptr)
		return false;
	coConstString identifier;
	coTRY(stream.ReadIdentifier(identifier), "Failed to read identifier");
	coField* field = type.FindField(identifier);
	if (field)
	{
		PassWhitespace(stream);
		coChar c = stream.ReadChar();
		coTRY(c == ':', "Invalid value assignment");
		coIntPtr fieldPtr = coIntPtr(object) + field->offset8;
		coASSERT(field->type);
		coTRY(ReadFieldValue(stream, (void*)fieldPtr, *field->type), nullptr);
	}
	else
	{
		coWARN("Field not found, ignored: " << identifier<< ", "<<stream.GetDebugName()<<"("<<line<<")");
	}
	return true;
}

coResult coDescReader::ReadFieldValue(coStringInputStream& stream, void* field, const coType& type)
{
	PassWhitespace(stream);
	if (type.IsCompatibleWith<coDynamicString>())
	{
		coConstString streamValue;
		coTRY(stream.ReadString(streamValue, '"'), "Invalid string");
		coDynamicString& fieldValue = *static_cast<coDynamicString*>(field);
		fieldValue = streamValue;
	}
	return true;
}

void coDescReader::PassWhitespace(coStringInputStream& stream)
{
	coUint32 passedLines = 0;
	stream.PassWhitespaceAndCountLines(passedLines);
	line += passedLines;
}
