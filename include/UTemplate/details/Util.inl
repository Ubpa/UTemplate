#pragma once

#include <cstdint>
#include <cassert>

namespace Ubpa::details {
	template<typename Void, template<typename...> typename T, typename... Ts>
	struct is_instantiable : std::false_type {};
	template<template<typename...> typename T, typename... Ts>
	struct is_instantiable<std::void_t<T<Ts...>>, T, Ts...> : std::true_type {};

	template<typename Void, typename T, typename... Args>
	struct is_list_initializable : std::false_type {};

	template<typename T, typename... Args>
	struct is_list_initializable<std::void_t<decltype(T{ std::declval<Args>()... })>, T, Args...> : std::true_type {};

	template<typename Void, typename T>
	struct is_defined_helper : std::false_type {};

	template<typename T>
	struct is_defined_helper<std::void_t<decltype(sizeof(T))>, T> : std::true_type {};

	template<std::size_t size>
	struct fnv1a_traits;

	template<>
	struct fnv1a_traits<4> {
		using type = std::uint32_t;
		static constexpr std::uint32_t offset = 2166136261;
		static constexpr std::uint32_t prime = 16777619;
	};

	template<>
	struct fnv1a_traits<8> {
		using type = std::uint64_t;
		static constexpr std::uint64_t offset = 14695981039346656037ull;
		static constexpr std::uint64_t prime = 1099511628211ull;
	};

	template<typename Void, typename T>
	struct is_function_pointer : std::false_type {};

	template<typename T>
	struct is_function_pointer<std::enable_if_t<std::is_pointer_v<T>&& std::is_function_v<std::remove_pointer_t<T>>>, T> : std::true_type {};

	struct has_virtual_base_void {};
	template<typename Void, typename Obj>
	struct has_virtual_base_helper : std::true_type {};
	template<typename Obj>
	struct has_virtual_base_helper<
		std::void_t<decltype(reinterpret_cast<has_virtual_base_void has_virtual_base_void::*>(std::declval<has_virtual_base_void Obj::*>()))>,
		Obj> : std::false_type {};

	template<typename Void, typename Base, typename Derived>
	struct is_virtual_base_of_helper : std::is_base_of<Base, Derived> {};
	template<typename Base, typename Derived>
	struct is_virtual_base_of_helper<
		std::void_t<decltype(static_cast<Derived*>(std::declval<Base*>()))>,
		Base, Derived> : std::false_type {};

	template <class Void, template<class...> class Op, class... Args>
	struct is_valid : std::false_type {};
	template <template<class...> class Op, class... Args>
	struct is_valid<std::void_t<Op<Args...>>, Op, Args...> : std::true_type {};
}

template<template<typename...> typename T, typename... Ts>
struct Ubpa::is_instantiable : details::is_instantiable<void, T, Ts...> {};

template<typename Instance, template<typename...> class T>
struct Ubpa::is_instance_of : std::false_type {};

template<typename... Args, template<typename...> class T>
struct Ubpa::is_instance_of<T<Args...>, T> : std::true_type {};

template<typename T, typename... Args>
struct Ubpa::is_list_initializable : details::is_list_initializable<void, T, Args...> {};

template<template<typename...> class TA, template<typename...> class TB>
struct Ubpa::is_same_typename_template : std::false_type {};

template<template<typename...> class T>
struct Ubpa::is_same_typename_template<T, T> : std::true_type {};

template<typename T>
struct Ubpa::is_defined : details::is_defined_helper<void, T> {};

template<typename T, typename U>
struct Ubpa::member_pointer_traits<T U::*> {
	using object = U;
	using value = T;
};

template<typename T>
struct Ubpa::is_typename_template_type : std::false_type {};

template<template<typename...>class T, typename... Ts>
struct Ubpa::is_typename_template_type<T<Ts...>> : std::true_type {};

template<typename T> struct Ubpa::IsIValue : std::false_type {};
template<typename T, T v> struct Ubpa::IsIValue<Ubpa::IValue<T, v>> : std::true_type {};

template<size_t N>
constexpr std::size_t Ubpa::lengthof(const char(&str)[N]) noexcept {
	static_assert(N > 0);
	assert(str[N - 1] == 0); // c-style string
	return N - 1;
}

constexpr std::size_t Ubpa::string_hash_seed(std::size_t seed, const char* str, std::size_t N) noexcept {
	using Traits = details::fnv1a_traits<sizeof(std::size_t)>;
	std::size_t value = seed;

	for (std::size_t i = 0; i < N; i++)
		value = (value ^ static_cast<Traits::type>(str[i])) * Traits::prime;

	return value;
}

constexpr std::size_t Ubpa::string_hash_seed(std::size_t seed, const char* curr) noexcept {
	using Traits = details::fnv1a_traits<sizeof(std::size_t)>;
	std::size_t value = seed;

	while (*curr) {
		value = (value ^ static_cast<Traits::type>(*(curr++))) * Traits::prime;
	}

	return value;
}

constexpr std::size_t Ubpa::string_hash(const char* str, std::size_t N) noexcept {
	using Traits = details::fnv1a_traits<sizeof(std::size_t)>;
	return string_hash_seed(Traits::offset, str, N);
}

constexpr std::size_t Ubpa::string_hash(const char* str) noexcept {
	using Traits = details::fnv1a_traits<sizeof(std::size_t)>;
	return string_hash_seed(Traits::offset, str);
}

template<typename T>
struct Ubpa::is_function_pointer : Ubpa::details::is_function_pointer<void, T> {};

template<typename T>
struct Ubpa::has_virtual_base : Ubpa::details::has_virtual_base_helper<void, T> {};

template<typename Base, typename Derived>
struct Ubpa::is_virtual_base_of : Ubpa::details::is_virtual_base_of_helper<void, Base, Derived> {};

template <template<class...> class Op, class... Args>
struct Ubpa::is_valid : Ubpa::details::is_valid<void, Op, Args...> {};

template<typename V1, typename Obj1, typename V2, typename Obj2>
constexpr bool Ubpa::member_pointer_equal(V1 Obj1::* p1, V2 Obj2::* p2) noexcept {
	if constexpr (std::is_same_v<Obj1, Obj2> && std::is_same_v<V1, V2>)
		return p1 == p2;
	else
		return false;
}
