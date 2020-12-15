#pragma once

#include <cstdint>

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

	template<typename>
	struct fnv1a_traits;

	template<>
	struct fnv1a_traits<std::uint32_t> {
		using type = std::uint32_t;
		static constexpr std::uint32_t offset = 2166136261;
		static constexpr std::uint32_t prime = 16777619;
	};

	template<>
	struct fnv1a_traits<std::uint64_t> {
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
}

template<template<typename...> typename T, typename... Ts>
struct Ubpa::is_instantiable : details::is_instantiable<void, T, Ts...> {};

template<typename Instance, template<typename...> class T>
struct Ubpa::is_instance_of : std::false_type {};

template<typename... Args, template<typename...> class T>
struct Ubpa::is_instance_of<T<Args...>, T> : std::true_type {};

template<template<typename, typename...>class Op, template<typename...>class Test>
struct Ubpa::AddIf {
private:
	template<bool test, typename I, typename... Args>
	struct Choose;
	template<typename I, typename... Args>
	struct Choose<true, I, Args...> : Op<I, Args...> {};
	template<typename I, typename... Args>
	struct Choose<false, I, Args...> : IType<I> {};
public:
	template<typename I, typename... Args>
	struct Ttype : Choose<Test<Args...>::value, I, Args...> {};
};

template<template<typename...>class Test>
struct Ubpa::Negate {
	template<typename... Args>
	struct Ttype : IValue<bool, !Test<Args...>::value> {};
};

template<typename T, typename... Args>
struct Ubpa::is_list_initializable : details::is_list_initializable<T, void, Args...> {};

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

constexpr size_t Ubpa::string_hash(const char* str, size_t N) noexcept {
	using Traits = details::fnv1a_traits<size_t>;
	auto value = Traits::offset;

	for (size_t i = 0; i < N; i++)
		value = (value ^ static_cast<Traits::type>(str[i])) * Traits::prime;

	return value;
}

constexpr size_t Ubpa::string_hash(const char* curr) noexcept {
	using Traits = details::fnv1a_traits<size_t>;
	auto value = Traits::offset;

	while (*curr != 0) {
		value = (value ^ static_cast<Traits::type>(*(curr++))) * Traits::prime;
	}

	return value;
}

template<typename T>
struct Ubpa::is_function_pointer : Ubpa::details::is_function_pointer<void, T> {};

template<typename T>
struct Ubpa::has_virtual_base : Ubpa::details::has_virtual_base_helper<void, T> {};

template<typename Base, typename Derived>
struct Ubpa::is_virtual_base_of : Ubpa::details::is_virtual_base_of_helper<void, Base, Derived> {};
