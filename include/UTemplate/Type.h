#pragma once

#include "Name.h"

#include <compare>

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

		explicit constexpr operator bool() const noexcept { return Valid(); }

	protected:
		constexpr std::strong_ordering operator<=>(const IDBase& rhs) const noexcept = default;

	private:
		std::size_t value;
	};

	class StrID : public IDBase {
	public:
		using IDBase::IDBase;
		constexpr std::strong_ordering operator<=>(const StrID& rhs) const { return IDBase::operator<=>(rhs); }
		friend constexpr bool operator==(const StrID& lhs, const StrID& rhs) noexcept { return lhs.operator<=>(rhs) == std::strong_ordering::equal; }
	};

	class TypeID : public IDBase {
	public:
		using IDBase::IDBase;
		using IDBase::Is;

		template<typename T>
		constexpr bool Is() const noexcept { return IDBase::Is(type_name<T>().View()); }
		constexpr std::strong_ordering operator<=>(const TypeID& rhs) const { return IDBase::operator<=>(rhs); }
		friend constexpr bool operator==(const TypeID& lhs, const TypeID& rhs) noexcept { return lhs.operator<=>(rhs) == std::strong_ordering::equal; }
	};

	template<typename T>
	constexpr TypeID TypeID_of = TypeID{ type_name<T>().View() };

	template<typename X, typename Y> struct TypeID_Less : std::bool_constant<TypeID_of<X> < TypeID_of<Y> > {
		static_assert(std::is_same_v<X, Y> || TypeID_of<X> != TypeID_of<Y>);
	};

	template<typename X, typename Y> constexpr bool TypeID_Less_v = TypeID_Less<X, Y>::value;

	class Type {
	public:
		constexpr Type() noexcept = default;
		constexpr Type(std::string_view name) noexcept : name{ name }, typeID{ name }{}
		constexpr Type(std::string_view name, TypeID typeID) noexcept : name{ name }, typeID{ typeID }
		{ assert(TypeID{ name } == typeID); }
		constexpr std::string_view GetName() const noexcept { return name; }
		constexpr TypeID GetTypeID() const noexcept { return typeID; }
		constexpr bool Valid() const noexcept { return !name.empty() && typeID.Valid(); }
		constexpr std::strong_ordering operator<=>(const Type& rhs) const noexcept { return typeID <=> rhs.typeID; }
		friend constexpr bool operator==(const Type& lhs, const Type& rhs) noexcept {
			if (lhs.typeID == rhs.typeID) {
				assert(lhs.name == rhs.name);
				return true;
			}
			
			return false;
		}
	private:
		std::string_view name;
		TypeID typeID;
	};

	template<typename T>
	constexpr Type Type_of = Type{ type_name<T>().View() };
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
