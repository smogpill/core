// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include "lang/result/coResult.h"
class coStringInputStream;

class coDescReader
{
public:
	template <class T>
	coResult Read(coStringInputStream& stream, T& object);
	coResult Read(coStringInputStream& stream, void* object, const coType& type);
private:
	coResult ReadObject(coStringInputStream& stream, void* object, const coType& type);
	coResult ReadExpression(coStringInputStream& stream, void* object, const coType& type);
	coResult ReadFieldValue(coStringInputStream& stream, void* field, const coType& type);
	void PassWhitespace(coStringInputStream& stream);

	coUint32 line = 0;
};

template <class T>
coResult coDescReader::Read(coStringInputStream& source, T& object)
{
	return Read(source, &object, *T::GetStaticType());
}
