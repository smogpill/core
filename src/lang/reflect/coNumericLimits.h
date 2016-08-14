// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class T>
class coNumericLimits
{
public:
	static T Min() { return std::numeric_limits<T>::min(); }
	static T Lowest() { return std::numeric_limits<T>::lowest(); }
	static T Max() { return std::numeric_limits<T>::max(); }
};

template <class To, class From>
coBool coTruncates(const From& _this)
{
	static_assert(std::numeric_limits<From>::is_integer, "");
	static_assert(std::numeric_limits<To>::is_integer, "");
	static coBool cond = std::numeric_limits<To>::is_signed; // To avoid "warning C4127: conditional expression is constant" with Visual Studio.
	if (cond)
	{
		return (!std::numeric_limits<From>::is_signed &&
			uintmax_t(_this) > uintmax_t(coNumericLimits<uintmax_t>::Max()) ||
			intmax_t(_this) < intmax_t(coNumericLimits<To>::Lowest()) ||
			intmax_t(_this) > intmax_t(coNumericLimits<To>::Max()));
	}
	else
	{
		return _this < 0 || uintmax_t(_this) > uintmax_t(coNumericLimits<To>::Max());
	}
}

template <class To>
coBool coTruncates(const coFloat& _this)
{
	coWARN_NOT_AVAILABLE();
	return true;
}

template <class To, class From>
void coNumericConvert(To& _to, const From& _this)
{
	coASSERT(!coTruncates<To>(_this));
	_to = static_cast<const To&>(_this);
}
