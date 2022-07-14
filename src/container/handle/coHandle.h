// Copyright(c) 2022 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class Tag, class Value, Value _invalidValue_ = ~Value(0)>
class coHandle
{
public:
	using UnderlyingType = Value;
	using HandleType = coHandle<Tag, Value, _invalidValue_>;
	coHandle() = default;
	coHandle(const HandleType& h) : default {}
	coHandle(const HandleType& value_) : value(value_) {}
	coBool operator==(const coHandle& h) const { return value == h.value; }
	coBool operator!=(const coHandle& h) const { return value != h.value; }
	operator Value() const { return value; }
	coBool isValid() const { return value != _invalidValue_; }

	static const HandleType invalid;
private:
	UnderlyingType value = _invalidValue_;
};

template <class Tag, class Value, Value _invalidValue_ = ~Value(0)>
inline const coHandle<Tag, Value, _invalidValue_> coHandle<Tag, Value, _invalidValue_>::invalid;
