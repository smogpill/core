// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
#include <debug/log/coAssert.h>

enum _coCharCategory : coUint16
{
	_coCharCategory_horzws = 1 << 0,	// '\t', '\f', '\v'
	_coCharCategory_vertws = 1 << 1,	// '\r', '\n'
	_coCharCategory_space = 1 << 2,		// ' '
	_coCharCategory_digit = 1 << 3,		// 0-9
	_coCharCategory_xletter = 1 << 4,	// a-f,A-F
	_coCharCategory_upper = 1 << 5,		// A-Z
	_coCharCategory_lower = 1 << 6,		// a-z
	_coCharCategory_under = 1 << 7,		// _
	_coCharCategory_period = 1 << 8,	// .
	_coCharCategory_rawdel = 1 << 9,	// {}[]#<>%:;?*+-/^&|~!=,"'
	_coCharCategory_punct = 1 << 10,	// `$@()
	_coCharCategory_xupper = _coCharCategory_xletter | _coCharCategory_upper,
	_coCharCategory_xlower = _coCharCategory_xletter | _coCharCategory_lower,
	_coCharCategory_letter = _coCharCategory_upper | _coCharCategory_lower,
};

extern const coUint16 _coCharCategoryTable[256];

coFORCE_INLINE coBool coIsASCII(coChar _c) { return _c <= 127; }
coFORCE_INLINE coBool coIsHorizontalWhitespace(coChar _c) { return (_coCharCategoryTable[_c] & (_coCharCategory_horzws | _coCharCategory_space)) != 0; }
coFORCE_INLINE coBool coIsVerticalWhitespace(coChar _c) { return (_coCharCategoryTable[_c] & _coCharCategory_vertws) != 0; }
coFORCE_INLINE coBool coIsWhitespace(coChar _c) { return (_coCharCategoryTable[_c] & (_coCharCategory_horzws | _coCharCategory_vertws | _coCharCategory_space)) != 0; }
coFORCE_INLINE coBool coIsIdentifierHeadCompatible(coChar _c) { return (_coCharCategoryTable[_c] & (_coCharCategory_upper | _coCharCategory_lower | _coCharCategory_under)) != 0; }
coFORCE_INLINE coBool coIsIdentifierBodyCompatible(coChar _c) { return (_coCharCategoryTable[_c] & (_coCharCategory_upper | _coCharCategory_lower | _coCharCategory_digit | _coCharCategory_under)) != 0; }
coFORCE_INLINE coBool coIsFileNameCompatible(coChar _c) { return (_coCharCategoryTable[_c] & (_coCharCategory_under | _coCharCategory_letter | _coCharCategory_space | _coCharCategory_digit | _coCharCategory_period | _coCharCategory_punct)) != 0; }
coFORCE_INLINE coBool coIsDigit(coChar _c) { return (_coCharCategoryTable[_c] & _coCharCategory_digit) != 0; }
coFORCE_INLINE coBool coIsDigitOrPeriod(coChar _c) { return (_coCharCategoryTable[_c] & (_coCharCategory_digit | _coCharCategory_period)) != 0; }
coFORCE_INLINE coBool coIsDigitOrPeriodOrUnder(coChar _c) { return (_coCharCategoryTable[_c] & (_coCharCategory_digit | _coCharCategory_period | _coCharCategory_under)) != 0; }
coFORCE_INLINE coBool coIsLowerCase(coChar _c) { return (_coCharCategoryTable[_c] & _coCharCategory_lower) != 0; }
coFORCE_INLINE coBool coIsUpperCase(coChar _c) { return (_coCharCategoryTable[_c] & _coCharCategory_upper) != 0; }
coFORCE_INLINE coBool coIsLetter(coChar _c) { return (_coCharCategoryTable[_c] & (_coCharCategory_upper | _coCharCategory_lower)) != 0; }
coFORCE_INLINE coBool coIsAlphaNumeric(coChar _c) { return (_coCharCategoryTable[_c] & (_coCharCategory_digit | _coCharCategory_letter)) != 0; }
coFORCE_INLINE coBool coIsHexDigit(coChar _c) { return (_coCharCategoryTable[_c] & (_coCharCategory_digit | _coCharCategory_xletter)) != 0; }
coFORCE_INLINE coUint8 coGetDigit(coChar _c) { coASSERT(coIsDigit(_c)); return _c - '0'; }

coFORCE_INLINE coChar coToLowerCase(coChar _c)
{
	if (coIsUpperCase(_c))
		return _c + 'a' - 'A';
	return _c;
}

coFORCE_INLINE coChar coToUpperCase(coChar _c)
{
	if (coIsLowerCase(_c))
		return _c + 'A' - 'a';
	return _c;
}
