#pragma once

#include "TSTR.h"
#include "Func.h"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstddef>
#include <iosfwd>
#include <iterator>
#include <limits>

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#include <cstdlib>
#endif

// Checks UXName_type compiler compatibility.
#if defined(__clang__) && __clang_major__ >= 5 || defined(__GNUC__) && __GNUC__ >= 7 || defined(_MSC_VER) && _MSC_VER >= 1910
#  undef  UXNAME_TYPE_SUPPORTED
#  define UXNAME_TYPE_SUPPORTED 1
#endif

// Checks UXName_type_rtti compiler compatibility.
#if defined(__clang__)
#  if __has_feature(cxx_rtti)
#    undef  UXNAME_TYPE_RTTI_SUPPORTED
#    define UXNAME_TYPE_RTTI_SUPPORTED 1
#  endif
#elif defined(__GNUC__)
#  if defined(__GXX_RTTI)
#    undef  UXNAME_TYPE_RTTI_SUPPORTED
#    define UXNAME_TYPE_RTTI_SUPPORTED 1
#  endif
#elif defined(_MSC_VER)
#  if defined(_CPPRTTI)
#    undef  UXNAME_TYPE_RTTI_SUPPORTED
#    define UXNAME_TYPE_RTTI_SUPPORTED 1
#  endif
#endif

// Checks UXName_enum compiler compatibility.
#if defined(__clang__) && __clang_major__ >= 5 || defined(__GNUC__) && __GNUC__ >= 9 || defined(_MSC_VER) && _MSC_VER >= 1910
#  undef  UXNAME_ENUM_SUPPORTED
#  define UXNAME_ENUM_SUPPORTED 1
#endif

// Checks UXName_enum compiler aliases compatibility.
#if defined(__clang__) && __clang_major__ >= 5 || defined(__GNUC__) && __GNUC__ >= 9 || defined(_MSC_VER) && _MSC_VER >= 1920
#  undef  UXNAME_ENUM_SUPPORTED_ALIASES
#  define UXNAME_ENUM_SUPPORTED_ALIASES 1
#endif

namespace Ubpa::details {
#if defined(_MSC_VER)
	template <typename T>
	struct identity {
		using type = T;
	};
#else
	template <typename T>
	using identity = T;
#endif

	template<typename T>
	constexpr auto func_signature_impl() noexcept {
#  if defined(__clang__)
		return std::string_view{ __PRETTY_FUNCTION__ };
#  elif defined(__GNUC__)
		return std::string_view{ __PRETTY_FUNCTION__ };
#  elif defined(_MSC_VER)
		return std::string_view{ __FUNCSIG__ };
#  endif
	}

	template<typename T>
	constexpr auto func_signature()noexcept {
		return TSTR(func_signature_impl<identity<T>>());
	}

	//
	// decode
	///////////

	template<typename T>
	struct to_typename_template_type {
		using type = T;
	};

	template<typename T>
	using to_typename_template_type_t = typename to_typename_template_type<T>::type;

	template<typename T>
	struct is_template_type : std::false_type {};

	template<template<typename...>class T, typename... Ts>
	struct is_template_type<T<Ts...>> : std::true_type {};
	template<typename T>
	static constexpr bool is_template_type_v = is_template_type<T>::value;

	template<typename T>
	struct is_integral_constant : std::false_type {};

	template<typename T, T v>
	struct is_integral_constant<std::integral_constant<T, v>> : std::true_type {};

	template<typename Str>
	constexpr auto remove_class_key(Str = {}) {
#if defined(__clang__)
		return Str{};
#elif defined(__GNUC__)
		return Str{};
#elif defined(_MSC_VER)
		constexpr auto n0 = remove_prefix(Str{}, TSTR("struct "));
		constexpr auto n1 = remove_prefix(n0, TSTR("enum "));
		constexpr auto n2 = remove_prefix(n1, TSTR("class "));
		return n2;
#endif
	}

	template<typename Str>
	constexpr std::pair<size_t, size_t> get_template_idx(Str = {}) {
		size_t k = 0;
		size_t a = static_cast<size_t>(-1);
		size_t b = static_cast<size_t>(-1);
		for (size_t i = 0; i < Str::name.size(); i++) {
			if (Str::name[i] == '<') {
				if (k == 0)
					a = i;
				++k;
			}
			else if (Str::name[i] == '>') {
				k--;
				if (k == 0)
					b = i;
			}
		}
		return { a,b };
	}

	template<typename Str>
	constexpr auto remove_template(Str = {}) {
		constexpr auto ab = get_template_idx<Str>();
		constexpr auto a = ab.first;
		constexpr auto b = ab.second;
		if constexpr (a != static_cast<size_t>(-1) && b != static_cast<size_t>(-1))
			return remove_template(replace<a, b - a + 1>(Str{}, TSTR("")));
		else
			return Str{};
	}
}

namespace Ubpa {
	template<typename T>
	constexpr auto raw_type_name()noexcept {
		constexpr auto sig = details::func_signature<T>();
#if defined(UXNAME_TYPE_SUPPORTED) && UXNAME_TYPE_SUPPORTED
#  if defined(__clang__)
		return remove_suffix<1>(remove_prefix<39>(sig));
#  elif defined(__GNUC__)
		return remove_suffix<1>(remove_prefix<54>(sig));
#  elif defined(_MSC_VER)
		return remove_suffix(remove_suffix<17>(remove_prefix<79>(sig)), TSTR(" "));
#  endif
#else
		return TSTR(""); // Unsupported compiler.
#endif
	}

	template<typename T>
	constexpr auto no_template_type_name()noexcept {
		// TODO custom namespace
		constexpr auto name = details::remove_class_key(raw_type_name<T>());
		return details::remove_template(name);
	}

	template<typename T>
	constexpr auto kernel_type_name()noexcept {
		constexpr auto name = details::remove_template(details::remove_class_key(raw_type_name<T>()));
		constexpr auto idx = find_last(name, TSTR(":"));
		if constexpr (idx != static_cast<size_t>(-1))
			return remove_prefix<idx+1>(name);
		else
			return name;
	}

	template<typename T>
	constexpr auto type_name() noexcept;

	template<typename T>
	struct template_args_name_impl;
	template<template<typename...>class T, typename... Ts>
	struct template_args_name_impl<T<Ts...>> {
		constexpr static auto get() noexcept {
			return concat_seq_seperator(TSTR(","), type_name<Ts>()...);
		}
	};

	template<typename T>
	constexpr auto template_args_name() noexcept {
		return template_args_name_impl<T>::get();
	}

	template<typename T>
	struct function_args_name_impl;
	template<typename... Ts>
	struct function_args_name_impl<TypeList<Ts...>> {
		constexpr static auto get() noexcept {
			return concat_seq_seperator(TSTR(","), type_name<Ts>()...);
		}
	};

	template<typename T>
	constexpr auto function_args_name() noexcept {
		return function_args_name_impl<T>::get();
	}

	template<typename T>
	struct member_pointer_to_object;
	template<typename T, typename U>
	struct member_pointer_to_object<T U::*> {
		using type = U;
	};
	template<typename T>
	struct member_pointer_to_value;
	template<typename T, typename U>
	struct member_pointer_to_value<T U::*> {
		using type = T;
	};

	template<typename T>
	constexpr auto type_name() noexcept {
		if constexpr (std::is_lvalue_reference_v<T>)
			return concat_seq(TSTR("&{"), type_name<std::remove_reference_t<T>>(), TSTR("}"));
		else if constexpr (std::is_rvalue_reference_v<T>)
			return concat_seq(TSTR("&&{"), type_name<std::remove_reference_t<T>>(), TSTR("}"));
		else if constexpr (std::is_const_v<T>)
			return concat_seq(TSTR("const{"), type_name<std::remove_const_t<T>>(), TSTR("}"));
		else if constexpr (std::is_volatile_v<T>)
			return concat_seq(TSTR("volatile{"), type_name<std::remove_volatile_t<T>>(), TSTR("}"));
		else if constexpr (std::is_member_pointer_v<T>) {
			using Obj = typename member_pointer_to_object<T>::type;
			using Value = typename member_pointer_to_value<T>::type;
			return concat_seq(TSTR("{"), type_name<Obj>(), TSTR("}::*{"), type_name<Value>());
		}
		else if constexpr (std::is_pointer_v<T>)
			return concat_seq(TSTR("*{"), type_name<std::remove_pointer_t<T>>(), TSTR("}"));
		else if constexpr (std::is_array_v<T>) {
			constexpr auto r = std::rank_v<T>;
			constexpr auto ex = std::extent_v<T, 0>;
			if constexpr (ex == 0)
				return concat_seq(TSTR("[]{"), type_name<std::remove_extent_t<T>>(), TSTR("}"));
			else
				return concat_seq(TSTR("["), TSTR("CONSTANT"), TSTR("]{"), type_name<std::remove_extent_t<T>>(), TSTR("}")); // TODO
		}
		else if constexpr (std::is_function_v<T>) {
			// const volatile &/&& noexcept
			using Ret = FuncTraits_Ret<T>;
			using ArgList = FuncTraits_ArgList<T>;
			return concat_seq(TSTR("("), function_args_name<ArgList>(), TSTR(")->"), type_name<Ret>());
		}
		else if constexpr (details::is_integral_constant<T>::value)
			return TSTR("CONSTANT");
		else {
			using U = details::to_typename_template_type_t<T>;
			if constexpr (details::is_template_type_v<U>)
				return concat_seq(no_template_type_name<T>(), TSTR("<"), template_args_name<U>(), TSTR(">"));
			else
				return details::remove_class_key(raw_type_name<T>());
		}
	}
}
