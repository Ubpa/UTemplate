#pragma once
#ifndef _UBPA_TEMPLATE_BASIC_H_
#define _UBPA_TEMPLATE_BASIC_H_

namespace Ubpa {
	namespace detail {
		template<template<typename...> typename T, typename AlwaysVoid, typename... Ts>
		struct is_instantiable :
			std::false_type {};

		template<template<typename...> typename T, typename... Ts>
		struct is_instantiable<T, std::void_t<T<Ts...>>, Ts...> :
			std::true_type {};

		// ------

		template<template<typename...> class T, template<typename...> class U, bool instantiable, typename... Args>
		struct is_same_template;

		template<template<typename...> class T, template<typename...> class U, typename... Args>
		struct is_same_template<T, U, false, Args...> : std::false_type {};

		template<template<typename...> class T, template<typename...> class U, typename... Args>
		struct is_same_template<T, U, true, Args...> {
			static constexpr bool value = std::is_same_v<T<Args...>, U<Args...>>;
		};
	}

	template<template<typename...> typename T, typename... Ts>
	using is_instantiable = detail::is_instantiable<T, void, Ts...>;
	template<template<typename...> typename T, typename... Ts>
	constexpr auto is_instantiable_v = is_instantiable<T, Ts...>::value;

	template<template<typename...> class T, template<typename...> class U, typename... Args>
	using is_same_template = detail::is_same_template<T, U,
		is_instantiable_v<T, Args...> && is_instantiable_v<U, Args...>, Args...>;

	template<template<typename...> class T, template<typename...> class U, typename... Args>
	constexpr bool is_same_template_v = is_same_template<T, U, Args...>::value;

	// is_instance_of
	template<typename Instance, template<typename...> class T>
	struct is_instance_of :
		std::false_type {};

	template<typename... Args, template<typename...> class T>
	struct is_instance_of<T<Args...>, T> :
		std::true_type {};

	template<typename... Args, template<typename...> class TInstance, template<typename...> class T>
	struct is_instance_of<TInstance<Args...>, T> {
		static constexpr bool value = is_same_template_v<TInstance, T, Args...>;
	};

	template<typename Instance, template<typename...> class T>
	constexpr bool is_instance_of_v = is_instance_of<Instance, T>::value;

	template<typename T, T... Vals>
	struct sequence { // sequence of integer parameters
		static_assert(std::is_integral_v<T>, "integer_sequence<T, I...> requires T to be an integral type.");

		using value_type = T;

		static constexpr size_t size() noexcept {
			return sizeof...(Vals);
		}
	};

	namespace detail {
		template<typename T, size_t I, T... Ns>
		struct make_sequence;

		template<typename T, T... Ns>
		struct make_sequence<T, 0, Ns...> : sequence<T, Ns...> {};

		template<typename T, size_t I, T... Ns>
		struct make_sequence : make_sequence<T, I - 1, I - 1, Ns...> {};
	}

	template<typename T, size_t I>
	using make_sequence = detail::make_sequence<T, I>;
}

#endif // !_UBPA_TEMPLATE_BASIC_H_
