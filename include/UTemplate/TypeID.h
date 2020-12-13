#pragma once

#include "Name.h"

namespace Ubpa {
	class StrID {
	public:
		static constexpr size_t InvalidValue() noexcept { return static_cast<size_t>(-1); }

		constexpr StrID() noexcept : value{ InvalidValue() } {}
		explicit constexpr StrID(size_t value) noexcept : value{ value } {}
		constexpr StrID(std::string_view str) noexcept : value{ string_hash(str) } {}
		template<size_t N>
		constexpr StrID(const char(&str)[N]) noexcept : value{ string_hash(str) } {}

		constexpr size_t GetValue() const noexcept { return value; }

		constexpr bool Valid() const noexcept { return value != InvalidValue(); }

		constexpr bool Is(std::string_view str) const noexcept { return operator==(StrID{ str }); }

		constexpr void Reset() noexcept { value = InvalidValue(); }

		explicit constexpr operator size_t() const noexcept { return value; }
		explicit constexpr operator bool() const noexcept { return Valid(); }

		constexpr bool operator< (const StrID& rhs) const noexcept { return value <  rhs.value; }
		constexpr bool operator<=(const StrID& rhs) const noexcept { return value <= rhs.value; }
		constexpr bool operator> (const StrID& rhs) const noexcept { return value >  rhs.value; }
		constexpr bool operator>=(const StrID& rhs) const noexcept { return value >= rhs.value; }
		constexpr bool operator==(const StrID& rhs) const noexcept { return value == rhs.value; }
		constexpr bool operator!=(const StrID& rhs) const noexcept { return value != rhs.value; }
	private:
		size_t value;
	};

	class TypeID : private StrID {
	public:
		using StrID::InvalidValue;
		using StrID::StrID;
		using StrID::GetValue;
		using StrID::Valid;
		using StrID::Reset;
		using StrID::operator size_t;
		using StrID::operator bool;

		using StrID::Is;

		template<typename T>
		constexpr bool Is() const noexcept { return operator==(TypeID::of<T>); }

		template<typename T>
		static constexpr TypeID of = TypeID{ type_name<T>().value };

		constexpr bool operator< (const TypeID& rhs) const noexcept { return StrID::operator< (rhs); }
		constexpr bool operator<=(const TypeID& rhs) const noexcept { return StrID::operator<=(rhs); }
		constexpr bool operator> (const TypeID& rhs) const noexcept { return StrID::operator> (rhs); }
		constexpr bool operator>=(const TypeID& rhs) const noexcept { return StrID::operator>=(rhs); }
		constexpr bool operator==(const TypeID& rhs) const noexcept { return StrID::operator==(rhs); }
		constexpr bool operator!=(const TypeID& rhs) const noexcept { return StrID::operator!=(rhs); }
	};

	template<typename X, typename Y> struct TypeID_Less : IValue<bool, TypeID::of<X> < TypeID::of<Y> >
	{ static_assert(std::is_same_v<X, Y> || TypeID::of<X> != TypeID::of<Y>); };

	template<typename X, typename Y> constexpr bool TypeID_Less_v = TypeID_Less<X, Y>::value;
}

template<>
struct std::hash<Ubpa::StrID> {
	size_t operator()(const Ubpa::StrID& ID) const noexcept {
		return ID.GetValue();
	}
};

template<>
struct std::hash<Ubpa::TypeID> {
	size_t operator()(const Ubpa::TypeID& ID) const noexcept {
		return ID.GetValue();
	}
};
