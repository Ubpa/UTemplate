#pragma once

#include <type_traits>

namespace Ubpa {
	template<typename T> struct IType { using type = T; };
	template<typename T, T V> struct IValue { static constexpr T value = V; };

	template<template<typename, typename...>class Op, template<typename...>class Test>
	struct AddIf;

	template<template<typename...>class Test>
	struct Negate;

	template<template<typename...> typename T, typename... Ts>
	struct is_instantiable;
	template<template<typename...> typename T, typename... Ts>
	constexpr bool is_instantiable_v = is_instantiable<T, Ts...>::value;

	template<template<typename...> class T, template<typename...> class U, typename... Args>
	struct is_same_template;
	template<template<typename...> class T, template<typename...> class U, typename... Args>
	constexpr bool is_same_template_v = is_same_template<T, U, Args...>::value;

	template<typename Instance, template<typename...> class T>
	struct is_instance_of;
	template<typename Instance, template<typename...> class T>
	constexpr bool is_instance_of_v = is_instance_of<Instance, T>::value;
}

namespace Ubpa::detail::Basic {
	template<template<typename...> typename T, typename AlwaysVoid, typename... Ts>
	struct is_instantiable;

	template<template<typename...> class T, template<typename...> class U, bool instantiable, typename... Args>
	struct is_same_template;
}

namespace Ubpa {
	template<template<typename...> typename T, typename... Ts>
	struct is_instantiable : detail::Basic::is_instantiable<T, void, Ts...> {};

	template<template<typename...> class T, template<typename...> class U, typename... Args>
	struct is_same_template : detail::Basic::is_same_template<T, U,
		is_instantiable_v<T, Args...> && is_instantiable_v<U, Args...>, Args...> {};

	// =================================================

	template<typename Instance, template<typename...> class T>
	struct is_instance_of : std::false_type {};

	template<typename... Args, template<typename...> class T>
	struct is_instance_of<T<Args...>, T> : std::true_type {};

	template<typename... Args, template<typename...> class TInstance, template<typename...> class T>
	struct is_instance_of<TInstance<Args...>, T>
		: IValue<bool, is_same_template_v<TInstance, T, Args...>> {};

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
}

namespace Ubpa::detail::Basic {
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
}
