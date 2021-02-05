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

		//
		// Traits
		///////////

		// primary

		constexpr bool IsVoid() const noexcept { return type_name_is_void(name); }
		constexpr bool IsNullptr() const noexcept { return type_name_is_null_pointer(name); }
		constexpr bool IsIntegral() const noexcept { return type_name_is_integral(name); }
		constexpr bool IsFloatingPoint() const noexcept { return type_name_is_floating_point(name); }
		constexpr bool IsArray() const noexcept { return type_name_is_array(name); }
		constexpr bool IsEnum() const noexcept { return type_name_is_enum(name); }
		constexpr bool IsUnion() const noexcept { return type_name_is_union(name); }
		constexpr bool IsFunction() const noexcept { return type_name_is_function(name); }
		constexpr bool IsPointer() const noexcept { return type_name_is_pointer(name); }
		constexpr bool IsLValueReference() const noexcept { return type_name_is_lvalue_reference(name); }
		constexpr bool IsRValueReference() const noexcept { return type_name_is_rvalue_reference(name); }
		constexpr bool IsMemberPointer() const noexcept { return type_name_is_member_pointer(name); }

		// composite
		constexpr bool IsArithmetic() const noexcept { return type_name_is_arithmetic(name); }
		constexpr bool IsFundamental() const noexcept { return type_name_is_fundamental(name); }

		// properties

		constexpr bool IsConst() const noexcept { return type_name_is_const(name); }
		constexpr bool IsVolatile() const noexcept { return type_name_is_volatile(name); }
		constexpr bool IsCV() const noexcept { return type_name_is_cv(name); }
		constexpr bool IsReference() const noexcept { return type_name_is_reference(name); }
		constexpr bool IsSign() const noexcept { return type_name_is_signed(name); }
		constexpr bool IsUnsigned() const noexcept { return type_name_is_unsigned(name); }
		constexpr bool IsBoundedArray() const noexcept { return type_name_is_bounded_array(name); }
		constexpr bool IsUnboundedArray() const noexcept { return type_name_is_unbounded_array(name); }
		constexpr std::size_t Rank() const noexcept { return type_name_rank(name); }
		constexpr std::size_t Extent() const noexcept { return type_name_extent(name); }

		// modification (clip)

		constexpr Type RemoveCV() const noexcept { return type_name_remove_cv(name); }
		constexpr Type RemoveConst() const noexcept { return type_name_remove_const(name); }
		constexpr Type RemoveTopMostVolatile() const noexcept { return type_name_remove_topmost_volatile(name); }
		constexpr Type RemoveReference() const noexcept { return type_name_remove_reference(name); }
		constexpr Type RemovePointer() const noexcept { return type_name_remove_pointer(name); }
		constexpr Type RemoveCVRef() const noexcept { return type_name_remove_cvref(name); }
		constexpr Type RemoveExtent() const noexcept { return type_name_remove_extent(name); }
		constexpr Type RemoveAllExtents() const noexcept { return type_name_remove_all_extents(name); }

		// modification (add, ID)

		constexpr TypeID ID_AddConst() const noexcept { return TypeID{ type_name_add_const_hash(name) }; }
		constexpr TypeID ID_AddVolatile() const noexcept { return TypeID{ type_name_add_volatile_hash(name) }; }
		constexpr TypeID ID_AddCV() const noexcept { return TypeID{ type_name_add_cv_hash(name) }; }
		constexpr TypeID ID_AddLValueReference() const noexcept { return TypeID{ type_name_add_lvalue_reference_hash(name) }; }
		// same with type_name_add_lvalue_reference_hash, but it won't change &&{T}
		constexpr TypeID ID_AddLValueReferenceWeak() const noexcept { return TypeID{ type_name_add_lvalue_reference_weak_hash(name) }; }
		constexpr TypeID ID_AddRValueReference() const noexcept { return TypeID{ type_name_add_rvalue_reference_hash(name) }; }
		constexpr TypeID ID_AddPointer() const noexcept { return TypeID{ type_name_add_pointer_hash(name) }; }
		constexpr TypeID ID_AddConstLValueReference() const noexcept { return TypeID{ type_name_add_const_lvalue_reference_hash(name) }; }
		constexpr TypeID ID_AddConstRValueReference() const noexcept { return TypeID{ type_name_add_const_rvalue_reference_hash(name) }; }

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
