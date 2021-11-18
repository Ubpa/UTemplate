#pragma once

#include "Name.hpp"

#include <compare>

namespace Ubpa {
	class NameID {
	public:
		static constexpr std::size_t InvalidValue() noexcept { return static_cast<std::size_t>(-1); }

		constexpr NameID() noexcept : value{ InvalidValue() } {}
		explicit constexpr NameID(std::size_t value) noexcept : value{ value } {}
		constexpr NameID(std::string_view str) noexcept : value{ string_hash(str) } {}
		template<std::size_t N>
		constexpr NameID(const char(&str)[N]) noexcept : value{ string_hash(str) } {}

		constexpr std::size_t GetValue() const noexcept { return value; }

		constexpr bool Valid() const noexcept { return value != InvalidValue(); }

		constexpr bool Is(std::string_view str) const noexcept { return value == NameID{ str }.GetValue(); }

		explicit constexpr operator bool() const noexcept { return Valid(); }

		constexpr std::strong_ordering operator<=>(const NameID& rhs) const noexcept = default;

	private:
		std::size_t value;
	};

	class TypeID {
	public:
		static constexpr std::size_t InvalidValue() noexcept { return static_cast<std::size_t>(-1); }

		constexpr TypeID() noexcept : nameID{ InvalidValue() } {}
		explicit constexpr TypeID(std::size_t value) noexcept : nameID{ value } {}
		constexpr TypeID(std::string_view str) noexcept : nameID{ str } {}
		template<std::size_t N>
		constexpr TypeID(const char(&str)[N]) noexcept : nameID{ str } {}

		constexpr std::size_t GetValue() const noexcept { return nameID.GetValue(); }

		constexpr bool Valid() const noexcept { return nameID.Valid(); }

		constexpr bool Is(std::string_view str) const noexcept { return nameID.Is(str); }

		template<typename T>
		constexpr bool Is() const noexcept { return GetValue() == TypeID{ type_name<T>().View() }.GetValue(); }

		explicit constexpr operator bool() const noexcept { return Valid(); }

		constexpr std::strong_ordering operator<=>(const TypeID& rhs) const noexcept = default;

	private:
		NameID nameID;
	};

	template<typename T>
	constexpr TypeID TypeID_of = TypeID{ type_name<T>().View() };

	template<typename X, typename Y> struct TypeID_Less : std::bool_constant<TypeID_of<X> < TypeID_of<Y> > {
		static_assert(std::is_same_v<X, Y> || TypeID_of<X> != TypeID_of<Y>);
	};

	template<typename X, typename Y> constexpr bool TypeID_Less_v = TypeID_Less<X, Y>::value;

	class Name {
	public:
		constexpr Name() noexcept = default;
		constexpr Name(std::string_view str) noexcept : str{ str }, nameID{ str }{}
		template<std::size_t N>
		constexpr Name(const char(&str)[N]) noexcept : Name{ std::string_view{ str} } {}
		constexpr Name(std::string_view str, NameID nameID) noexcept : str{ str }, nameID{ nameID }
		{ assert(NameID{ str } == nameID); }
		constexpr std::string_view GetView() const noexcept { return str; }
		constexpr operator std::string_view() const noexcept { return GetView(); }
		constexpr NameID GetID() const noexcept { return nameID; }
		constexpr operator NameID() const noexcept { return GetID(); }
		constexpr bool Valid() const noexcept { return !str.empty() && nameID.Valid(); }
		constexpr operator bool() const noexcept { return Valid(); }
		constexpr std::strong_ordering operator<=>(const Name& rhs) const noexcept { return nameID <=> rhs.nameID; }
		friend constexpr bool operator==(const Name& lhs, const Name& rhs) noexcept {
			if (lhs.nameID == rhs.nameID) {
				assert(lhs.str == rhs.str);
				return true;
			}

			return false;
		}
		constexpr bool Is(std::string_view in_str) const noexcept { return str == in_str; }
	private:
		std::string_view str;
		NameID nameID;
	};

	class Type {
	public:
		constexpr Type() noexcept = default;
		constexpr Type(std::string_view str) noexcept : name{ str } {}
		template<std::size_t N>
		constexpr Type(const char(&str)[N]) noexcept : name{ std::string_view{ str} } {}
		constexpr Type(std::string_view str, TypeID typeID) noexcept : name{ str, NameID{ typeID.GetValue() } } {}
		constexpr std::string_view GetName() const noexcept { return name.GetView(); }
		constexpr operator std::string_view() const noexcept { return GetName(); }
		constexpr TypeID GetID() const noexcept { return TypeID{ name.GetID().GetValue() }; }
		constexpr operator TypeID() const noexcept { return GetID(); }
		constexpr bool Valid() const noexcept { return name.Valid(); }
		constexpr operator bool() const noexcept { return Valid(); }
		constexpr std::strong_ordering operator<=>(const Type& rhs) const noexcept = default;

		template<typename T>
		constexpr bool Is() const noexcept {
			if (GetID() == TypeID_of<T>) {
				assert(name.Is(type_name<T>().View()));
				return true;
			}
			return false;
		}
		constexpr bool Is(std::string_view str) const noexcept { return name.Is(str); }

		//
		// Traits
		///////////

		// primary

		constexpr bool IsVoid() const noexcept { return type_name_is_void(name.GetView()); }
		constexpr bool IsNullptr() const noexcept { return type_name_is_null_pointer(name.GetView()); }
		constexpr bool IsIntegral() const noexcept { return type_name_is_integral(name.GetView()); }
		constexpr bool IsFloatingPoint() const noexcept { return type_name_is_floating_point(name.GetView()); }
		constexpr bool IsArray() const noexcept { return type_name_is_array(name.GetView()); }
		constexpr bool IsEnum() const noexcept { return type_name_is_enum(name.GetView()); }
		constexpr bool IsUnion() const noexcept { return type_name_is_union(name.GetView()); }
		constexpr bool IsFunction() const noexcept { return type_name_is_function(name.GetView()); }
		constexpr bool IsPointer() const noexcept { return type_name_is_pointer(name.GetView()); }
		constexpr bool IsLValueReference() const noexcept { return type_name_is_lvalue_reference(name.GetView()); }
		constexpr bool IsRValueReference() const noexcept { return type_name_is_rvalue_reference(name.GetView()); }
		constexpr bool IsMemberPointer() const noexcept { return type_name_is_member_pointer(name.GetView()); }

		// composite
		constexpr bool IsArithmetic() const noexcept { return type_name_is_arithmetic(name.GetView()); }
		constexpr bool IsFundamental() const noexcept { return type_name_is_fundamental(name.GetView()); }

		// properties

		constexpr bool IsConst() const noexcept { return type_name_is_const(name.GetView()); }
		constexpr bool IsReadOnly() const noexcept { return type_name_is_read_only(name.GetView()); }
		constexpr bool IsVolatile() const noexcept { return type_name_is_volatile(name.GetView()); }
		constexpr bool IsCV() const noexcept { return type_name_is_cv(name.GetView()); }
		constexpr bool IsReference() const noexcept { return type_name_is_reference(name.GetView()); }
		constexpr bool IsSign() const noexcept { return type_name_is_signed(name.GetView()); }
		constexpr bool IsUnsigned() const noexcept { return type_name_is_unsigned(name.GetView()); }
		constexpr bool IsBoundedArray() const noexcept { return type_name_is_bounded_array(name.GetView()); }
		constexpr bool IsUnboundedArray() const noexcept { return type_name_is_unbounded_array(name.GetView()); }
		constexpr std::size_t Rank() const noexcept { return type_name_rank(name.GetView()); }
		constexpr std::size_t Extent() const noexcept { return type_name_extent(name.GetView()); }
		constexpr CVRefMode GetCVRefMode() const noexcept { return type_name_cvref_mode(name.GetView()); }

		// modification (clip)

		constexpr std::string_view Name_RemoveCV() const noexcept { return type_name_remove_cv(name.GetView()); }
		constexpr std::string_view Name_RemoveConst() const noexcept { return type_name_remove_const(name.GetView()); }
		constexpr std::string_view Name_RemoveTopMostVolatile() const noexcept { return type_name_remove_topmost_volatile(name.GetView()); }
		constexpr std::string_view Name_RemoveLValueReference() const noexcept { return type_name_remove_lvalue_reference(name.GetView()); }
		constexpr std::string_view Name_RemoveRValueReference() const noexcept { return type_name_remove_rvalue_reference(name.GetView()); }
		constexpr std::string_view Name_RemoveReference() const noexcept { return type_name_remove_reference(name.GetView()); }
		constexpr std::string_view Name_RemovePointer() const noexcept { return type_name_remove_pointer(name.GetView()); }
		constexpr std::string_view Name_RemoveCVRef() const noexcept { return type_name_remove_cvref(name.GetView()); }
		constexpr std::string_view Name_RemoveExtent() const noexcept { return type_name_remove_extent(name.GetView()); }
		constexpr std::string_view Name_RemoveAllExtents() const noexcept { return type_name_remove_all_extents(name.GetView()); }

		constexpr Type RemoveCV() const noexcept { return FastGetType(Name_RemoveCV()); }
		constexpr Type RemoveConst() const noexcept { return FastGetType(Name_RemoveConst()); }
		constexpr Type RemoveTopMostVolatile() const noexcept { return FastGetType(Name_RemoveTopMostVolatile()); }
		constexpr Type RemoveLValueReference() const noexcept { return FastGetType(Name_RemoveLValueReference()); }
		constexpr Type RemoveRValueReference() const noexcept { return FastGetType(Name_RemoveRValueReference()); }
		constexpr Type RemoveReference() const noexcept { return FastGetType(Name_RemoveReference()); }
		constexpr Type RemovePointer() const noexcept { return FastGetType(Name_RemovePointer()); }
		constexpr Type RemoveCVRef() const noexcept { return FastGetType(Name_RemoveCVRef()); }
		constexpr Type RemoveExtent() const noexcept { return FastGetType(Name_RemoveExtent()); }
		constexpr Type RemoveAllExtents() const noexcept { return FastGetType(Name_RemoveAllExtents()); }

		// modification (add, ID)

		constexpr TypeID ID_AddConst() const noexcept { return TypeID{ type_name_add_const_hash(name.GetView()) }; }
		constexpr TypeID ID_AddVolatile() const noexcept { return TypeID{ type_name_add_volatile_hash(name.GetView()) }; }
		constexpr TypeID ID_AddCV() const noexcept { return TypeID{ type_name_add_cv_hash(name.GetView()) }; }
		constexpr TypeID ID_AddLValueReference() const noexcept { return TypeID{ type_name_add_lvalue_reference_hash(name.GetView()) }; }
		// same with type_name_add_lvalue_reference_hash, but it won't change &&{T}
		constexpr TypeID ID_AddLValueReferenceWeak() const noexcept { return TypeID{ type_name_add_lvalue_reference_weak_hash(name.GetView()) }; }
		constexpr TypeID ID_AddRValueReference() const noexcept { return TypeID{ type_name_add_rvalue_reference_hash(name.GetView()) }; }
		constexpr TypeID ID_AddPointer() const noexcept { return TypeID{ type_name_add_pointer_hash(name.GetView()) }; }
		constexpr TypeID ID_AddConstLValueReference() const noexcept { return TypeID{ type_name_add_const_lvalue_reference_hash(name.GetView()) }; }
		constexpr TypeID ID_AddConstRValueReference() const noexcept { return TypeID{ type_name_add_const_rvalue_reference_hash(name.GetView()) }; }

	private:
		// avoid hash
		constexpr Type FastGetType(std::string_view str) const noexcept { return name.GetView().data() == str.data() ? *this : Type{ str }; }

		Name name;
	};

	template<typename T>
	constexpr Type Type_of = Type{ type_name<T>().View() };

	template<typename X, typename Y> struct Type_Less : std::bool_constant < Type_of<X> < Type_of<Y> > {
		static_assert(std::is_same_v<X, Y> || Type_of<X> != Type_of<Y>);
	};

	template<typename X, typename Y> constexpr bool Type_Less_v = Type_Less<X, Y>::value;

	template<std::size_t N>
	class TempNameIDs : public TempArray<NameID, N> {
		using TempArray<NameID, N>::TempArray;
	};
	template<std::size_t N>
	class TempTypeIDs : public TempArray<TypeID, N> {
		using TempArray<TypeID, N>::TempArray;
	};
	template<std::size_t N>
	class TempNames : public TempArray<Name, N> {
		using TempArray<Name, N>::TempArray;
	};
	template<std::size_t N>
	class TempTypes : public TempArray<Type, N> {
		using TempArray<Type, N>::TempArray;
	};
	template<typename... Ts> TempNameIDs(Ts...)->TempNameIDs<sizeof...(Ts)>;
	template<typename... Ts> TempTypeIDs(Ts...)->TempTypeIDs<sizeof...(Ts)>;
	template<typename... Ts> TempNames(Ts...)->TempNames<sizeof...(Ts)>;
	template<typename... Ts> TempTypes(Ts...)->TempTypes<sizeof...(Ts)>;
	template<typename... Ts>
	constexpr auto TypeIDs_of = TempTypeIDs{ TypeID_of<Ts>... };
	template<typename... Ts>
	constexpr auto Types_of = TempTypes{ Type_of<Ts>... };
}

template<>
struct std::hash<Ubpa::NameID> {
	std::size_t operator()(const Ubpa::NameID& ID) const noexcept {
		return ID.GetValue();
	}
};

template<>
struct std::hash<Ubpa::TypeID> {
	std::size_t operator()(const Ubpa::TypeID& ID) const noexcept {
		return ID.GetValue();
	}
};

template<>
struct std::hash<Ubpa::Name> {
	std::size_t operator()(const Ubpa::Name& name) const noexcept {
		return name.GetID().GetValue();
	}
};

template<>
struct std::hash<Ubpa::Type> {
	std::size_t operator()(const Ubpa::Type& type) const noexcept {
		return type.GetID().GetValue();
	}
};
