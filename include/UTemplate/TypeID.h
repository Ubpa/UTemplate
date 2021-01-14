#pragma once

#include "Name.h"

namespace Ubpa {
	class IDBase {
	public:
		static constexpr std::size_t InvalidValue() noexcept { return static_cast<std::size_t>(-1); }

		constexpr IDBase() noexcept : value{ InvalidValue() } {}
		explicit constexpr IDBase(std::size_t value) noexcept : value{ value } {}
		constexpr IDBase(std::string_view str) noexcept : value{ string_hash(str) } {}
		template<std::size_t N>
		constexpr IDBase(const char(&str)[N]) noexcept : value{ string_hash(str) } {}

		constexpr std::size_t GetValue() const noexcept { return value; }

		constexpr bool Valid() const noexcept { return value != InvalidValue(); }

		constexpr bool Is(std::string_view str) const noexcept { return value == IDBase{ str }.GetValue(); }

		constexpr void Reset() noexcept { value = InvalidValue(); }

		explicit constexpr operator bool() const noexcept { return Valid(); }

	private:
		std::size_t value;
	};

	class StrID : public IDBase {
	public:
		using IDBase::IDBase;

		constexpr bool operator< (const StrID& rhs) const noexcept { return GetValue() <  rhs.GetValue(); }
		constexpr bool operator<=(const StrID& rhs) const noexcept { return GetValue() <= rhs.GetValue(); }
		constexpr bool operator> (const StrID& rhs) const noexcept { return GetValue() >  rhs.GetValue(); }
		constexpr bool operator>=(const StrID& rhs) const noexcept { return GetValue() >= rhs.GetValue(); }
		constexpr bool operator==(const StrID& rhs) const noexcept { return GetValue() == rhs.GetValue(); }
		constexpr bool operator!=(const StrID& rhs) const noexcept { return GetValue() != rhs.GetValue(); }
	};

	class TypeID : public IDBase {
	public:
		using IDBase::IDBase;
		using IDBase::Is;

		template<typename T>
		constexpr bool Is() const noexcept { return IDBase::Is(type_name<T>().View()); }
		
		constexpr bool operator< (const TypeID& rhs) const noexcept { return GetValue() <  rhs.GetValue(); }
		constexpr bool operator<=(const TypeID& rhs) const noexcept { return GetValue() <= rhs.GetValue(); }
		constexpr bool operator> (const TypeID& rhs) const noexcept { return GetValue() >  rhs.GetValue(); }
		constexpr bool operator>=(const TypeID& rhs) const noexcept { return GetValue() >= rhs.GetValue(); }
		constexpr bool operator==(const TypeID& rhs) const noexcept { return GetValue() == rhs.GetValue(); }
		constexpr bool operator!=(const TypeID& rhs) const noexcept { return GetValue() != rhs.GetValue(); }
	};

	template<typename T>
	constexpr TypeID TypeID_of = TypeID{ type_name<T>().View() };

	template<typename X, typename Y> struct TypeID_Less : IValue<bool, TypeID_of<X> < TypeID_of<Y> >
	{ static_assert(std::is_same_v<X, Y> || TypeID_of<X> != TypeID_of<Y>); };

	template<typename X, typename Y> constexpr bool TypeID_Less_v = TypeID_Less<X, Y>::value;
}

template<>
struct std::hash<Ubpa::StrID> {
	std::size_t operator()(const Ubpa::StrID& ID) const noexcept {
		return ID.GetValue();
	}
};

template<>
struct std::hash<Ubpa::TypeID> {
	std::size_t operator()(const Ubpa::TypeID& ID) const noexcept {
		return ID.GetValue();
	}
};
