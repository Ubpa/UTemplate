#pragma once

#include <type_traits>

#include "Concept.h"

namespace Ubpa {
	template<typename T> struct IType { using type = T; };
	template<typename T, T V> struct IValue { static constexpr T value = V; };

	template<template<typename, typename...>class Op, template<typename...>class Test>
	struct AddIf; // Ttype

	template<template<typename...>class Test>
	struct Negate; // Ttype

	template<template<typename...> typename T, typename... Ts>
	struct is_instantiable;
	template<template<typename...> typename T, typename... Ts>
	constexpr bool is_instantiable_v = is_instantiable<T, Ts...>::value;

	template<template<typename...> class TA, template<typename...> class TB>
	struct is_same_template;
	template<template<typename...> class TA, template<typename...> class TB>
	constexpr bool is_same_template_v = is_same_template<TA, TB>::value;

	template<typename Instance, template<typename...> class T>
	struct is_instance_of;
	template<typename Instance, template<typename...> class T>
	constexpr bool is_instance_of_v = is_instance_of<Instance, T>::value;

	template<typename T, typename... Args> struct is_derived_constructible;
	template<typename T, typename... Args> constexpr bool is_derived_constructible_v
		= is_derived_constructible<T, Args...>::value;

	template<typename T, typename... Args>
	struct is_list_initializable;
	template<typename T, typename... Args>
	static constexpr bool is_list_initializable_v = is_list_initializable<T, Args...>::value;

	template<typename T>
	struct is_defined;
	template<typename T>
	static constexpr bool is_defined_v = is_defined<T>::value;
}

namespace Ubpa::detail::Basic_ {
	template<template<typename...> typename T, typename AlwaysVoid, typename... Ts>
	struct is_instantiable;

	template<template<typename...> class T, template<typename...> class U, bool instantiable, typename... Args>
	struct is_same_template;

	template<typename T, typename Enabler /*=void*/, typename... Args>
	struct is_list_initializable;

	template<typename Void, typename T>
	struct is_defined_helper;
}

namespace Ubpa {
	template<template<typename...> typename T, typename... Ts>
	struct is_instantiable : detail::Basic_::is_instantiable<T, void, Ts...> {};

	// =================================================

	template<typename Instance, template<typename...> class T>
	struct is_instance_of : std::false_type {};

	template<typename... Args, template<typename...> class T>
	struct is_instance_of<T<Args...>, T> : std::true_type {};

	// =================================================

	template<template<typename, typename...>class Op, template<typename...>class Test>
	struct AddIf {
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
	struct Negate {
		template<typename... Args>
		struct Ttype : IValue<bool, !Test<Args...>::value> {};
	};

	// =================================================

	template<typename T, typename... Args>
	struct is_derived_constructible {
	private:
		template<typename Base> struct Derived : Base { };
		template <typename, typename = void> struct test : std::false_type {};
		template <typename U>
		struct test<U, decltype(void(Derived<U>(std::declval<Args>()...)))> : std::true_type {};

	public:
		static constexpr bool value = test<T>::value;
	};

	// =================================================

	template<typename T, typename... Args>
	struct is_list_initializable : detail::Basic_::is_list_initializable<T, void, Args...> {};

	// =================================================

	template<template<typename...> class TA, template<typename...> class TB>
	struct is_same_template : std::false_type {};

	template<template<typename...> class T>
	struct is_same_template<T, T> : std::true_type {};

	// =================================================

	template<typename T>
	struct is_defined : detail::Basic_::is_defined_helper<void, T> {};
}

namespace Ubpa::detail::Basic_ {
	template<template<typename...> typename T, typename AlwaysVoid, typename... Ts>
	struct is_instantiable : std::false_type {};

	template<template<typename...> typename T, typename... Ts>
	struct is_instantiable<T, std::void_t<T<Ts...>>, Ts...> : std::true_type {};

	// =================================================

	template<template<typename...> class T, template<typename...> class U, typename... Args>
	struct is_same_template<T, U, false, Args...> : std::false_type {};

	template<template<typename...> class T, template<typename...> class U, typename... Args>
	struct is_same_template<T, U, true, Args...>
		: IValue<bool, std::is_same_v<T<Args...>, U<Args...>>> {};

	template<typename T, typename Enabler, typename... Args>
	struct is_list_initializable : std::false_type {};

	template<typename T, typename... Args>
	struct is_list_initializable < T, std::void_t<decltype(T{ std::declval<Args>()... }) > , Args... > : std::true_type {};

	// =================================================

	template<typename Void, typename T>
	struct is_defined_helper : std::false_type {};

	template<typename T>
	struct is_defined_helper<std::void_t<decltype(sizeof(T))>, T> : std::true_type {};
}
