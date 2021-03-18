#pragma once

#include <type_traits>
#include <string_view>
#include <span>

namespace Ubpa {
	template<typename T>
	constexpr bool always_false = false;

	template<typename T, T V> struct IValue { static constexpr T value = V; };
	template<typename T> struct IsIValue;
	template<typename T> constexpr bool IsIValue_v = IsIValue<T>::value;
	template<auto V> using IValue_of = IValue<decltype(V), V>;

	template<typename...> struct typename_template_type;

	template<typename T> struct is_typename_template_type;
	template<typename T> static constexpr bool is_typename_template_type_v = is_typename_template_type<T>::value;

	// use IValue to replace integral value in template arguments
	// we provide some partial template specializations (see details/ToTTType.inl for more details)
	// [example]
	// template<typename T, std::size_t N>
	// struct Array;
	// to_typename_template_type_t<Array<T, N>> == typename_template_type<T, IValue<std::size_t, N>>
	template<typename T> struct to_typename_template_type : std::type_identity<T> {};
	template<typename T> using to_typename_template_type_t = typename to_typename_template_type<T>::type;

	// type object
	// type value
	template<typename T> struct member_pointer_traits;
	template<typename T> using member_pointer_traits_object = typename member_pointer_traits<T>::object;
	template<typename T> using member_pointer_traits_value = typename member_pointer_traits<T>::value;

	template<template<typename...> typename T, typename... Ts>
	struct is_instantiable;
	template<template<typename...> typename T, typename... Ts>
	constexpr bool is_instantiable_v = is_instantiable<T, Ts...>::value;

	template<template<typename...> class TA, template<typename...> class TB>
	struct is_same_typename_template;
	template<template<typename...> class TA, template<typename...> class TB>
	constexpr bool is_same_typename_template_v = is_same_typename_template<TA, TB>::value;

	template<typename Instance, template<typename...> class T>
	struct is_instance_of;
	template<typename Instance, template<typename...> class T>
	constexpr bool is_instance_of_v = is_instance_of<Instance, T>::value;

	template<typename T, typename... Args>
	struct is_list_initializable;
	template<typename T, typename... Args>
	static constexpr bool is_list_initializable_v = is_list_initializable<T, Args...>::value;

	template<typename T> struct is_defined;
	template<typename T> static constexpr bool is_defined_v = is_defined<T>::value;

	template<typename T> struct has_virtual_base;
	template<typename T> constexpr bool has_virtual_base_v = has_virtual_base<T>::value;

	template<typename Base, typename Derived> struct is_virtual_base_of;
	template<typename Base, typename Derived> constexpr bool is_virtual_base_of_v = is_virtual_base_of<Base, Derived>::value;

	template<size_t N>
	constexpr std::size_t lengthof(const char(&str)[N]) noexcept;

	constexpr std::size_t string_hash_seed(std::size_t seed, const char* str, std::size_t N) noexcept;
	constexpr std::size_t string_hash_seed(std::size_t seed, std::string_view str) noexcept { return string_hash_seed(seed, str.data(), str.size()); }
	template<std::size_t N>
	constexpr std::size_t string_hash_seed(std::size_t seed, const char(&str)[N]) noexcept { return string_hash_seed(seed, str, N - 1); }
	constexpr std::size_t string_hash_seed(std::size_t seed, const char* str) noexcept;

	constexpr std::size_t string_hash(const char* str, std::size_t N) noexcept;
	constexpr std::size_t string_hash(std::string_view str) noexcept { return string_hash(str.data(), str.size()); }
	template<std::size_t N>
	constexpr std::size_t string_hash(const char(&str)[N]) noexcept { return string_hash(str, N - 1); }
	constexpr std::size_t string_hash(const char* str) noexcept;

	template<typename T> struct is_function_pointer;
	template<typename T> constexpr bool is_function_pointer_v = is_function_pointer<T>::value;

	template <template<class...> class Op, class... Args>
	struct is_valid;
	template <template<class...> class Op, class... Args>
	constexpr bool is_valid_v = is_valid<Op, Args...>::value;

	template<typename V1, typename Obj1, typename V2, typename Obj2>
	constexpr bool member_pointer_equal(V1 Obj1::* p1, V2 Obj2::* p2) noexcept;

	template<typename Y>
	struct is_same_with {
		template<typename X>
		struct Ttype : std::is_same<X, Y> {};
	};

	enum class ReferenceMode {
		None,
		Left,
		Right
	};

	enum class CVRefMode : std::uint8_t {
		None          = 0b0000,
		Left          = 0b0001,
		Right         = 0b0010,
		Const         = 0b0100,
		ConstLeft     = 0b0101,
		ConstRight    = 0b0110,
		Volatile      = 0b1000,
		VolatileLeft  = 0b1001,
		VolatileRight = 0b1010,
		CV            = 0b1100,
		CVLeft        = 0b1101,
		CVRight       = 0b1110,
	};

	constexpr bool CVRefMode_IsLeft(CVRefMode mode) noexcept {
		return static_cast<std::uint8_t>(mode) & 0b0001;
	}
	constexpr bool CVRefMode_IsRight(CVRefMode mode) noexcept {
		return static_cast<std::uint8_t>(mode) & 0b0010;
	}
	constexpr bool CVRefMode_IsConst(CVRefMode mode) noexcept {
		return static_cast<std::uint8_t>(mode) & 0b0100;
	}
	constexpr bool CVRefMode_IsVolatile(CVRefMode mode) noexcept {
		return static_cast<std::uint8_t>(mode) & 0b1000;
	}

	template<typename T, std::size_t N>
	class TempArray {
	public:
		template<typename... Elems>
		constexpr TempArray(Elems&&... elems) : data{ static_cast<T>(elems)... } {}

		constexpr operator std::add_lvalue_reference_t<T[N]>()& { return data; }
		constexpr operator std::add_lvalue_reference_t<const T[N]>()const& { return data; }
		constexpr operator std::add_rvalue_reference_t<T[N]>()&& { return std::move(data); }
		constexpr operator std::add_rvalue_reference_t<const T[N]>()const&& { return std::move(data); }

		constexpr operator std::span<T>() { return data; }
		constexpr operator std::span<const T>()const { return data; }
		constexpr operator std::span<T, N>() { return data; }
		constexpr operator std::span<const T, N>()const { return data; }
	private:
		T data[N];
	};

	template<typename T, typename... Ts>
	TempArray(T, Ts...)->TempArray<T, sizeof...(Ts) + 1>;
}

#include "details/ToTTType.inl"
#include "details/Util.inl"
