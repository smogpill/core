// Copyright(c) 2021 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once
class coVec3;
class coVec4;

class coColor
{
public:
	coFORCE_INLINE explicit coColor(coNullPtr) {}
	coFORCE_INLINE coColor() : r(0), g(0), b(0), a(255) {}
	coFORCE_INLINE coColor(coUint8 r, coUint8 g, coUint8 b, coUint8 a = 255) : r(r), g(g), b(b), a(a) {}
	coFORCE_INLINE coColor(const coColor& rgb, coUint8 a) : r(rgb.r), g(rgb.g), b(rgb.b), a(a) {}
	coFORCE_INLINE coColor(coUint32 rgba) { reinterpret_cast<coUint32&>(*this) = rgba; }
	operator coVec3() const;
	operator coVec4() const;
	coFORCE_INLINE operator coUint32() const { return reinterpret_cast<const coUint32&>(r); }

	coUint8 r;
	coUint8 g;
	coUint8 b;
	coUint8 a;

	static const coColor s_white;
	static const coColor s_black;
	static const coColor s_red;
	static const coColor s_green;
	static const coColor s_blue;
	static const coColor s_coolRed;
	static const coColor s_coolGreen;
	static const coColor s_coolBlue;
	static const coColor s_coolOrange;
};
