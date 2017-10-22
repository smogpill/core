// Copyright(c) 2016 Jounayd Id Salah
// Distributed under the MIT License (See accompanying file LICENSE.md file or copy at http://opensource.org/licenses/MIT).
#pragma once

template <class MostDerived, class OwnerType, typename MemberType>
coFORCE_INLINE constexpr coIntPtr coGetFieldOffset(MemberType OwnerType::* _field)
{
	return reinterpret_cast<coIntPtr> (&(reinterpret_cast<const MostDerived*>(0)->*_field));
}

template <typename MemberType>
coFORCE_INLINE constexpr const MemberType& coGetFieldFromOffset(const void* _object, coIntPtr _offset)
{
	return *(reinterpret_cast<const MemberType*>(reinterpret_cast<const coByte*>(_object) + _offset));
}

template <typename MemberType>
coFORCE_INLINE constexpr MemberType& coGetMemberFromOffset(void* _object, coIntPtr _offset)
{
	return *(reinterpret_cast<MemberType*>(reinterpret_cast<coByte*>(_object) + _offset));
}

template <class OwnerType, typename MemberType>
coFORCE_INLINE constexpr const OwnerType& coGetObjectFromMemberOffset(const MemberType& _member, coIntPtr  _offset)
{
	return *(reinterpret_cast<const OwnerType*>(reinterpret_cast<const coByte*>(&_member) - _offset));
}

template <class OwnerType, typename MemberType>
coFORCE_INLINE constexpr OwnerType& coGetObjectFromMemberOffset(MemberType& _member, coIntPtr  _offset)
{
	return *(reinterpret_cast<OwnerType*>(reinterpret_cast<coByte*>(&_member) - _offset));
}
