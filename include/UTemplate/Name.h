#ifndef UBPA_UXNAME_HPP
#define UBPA_UXNAME_HPP

#include "Func.h"

#define UXNAME_VERSION_MAJOR 0
#define UXNAME_VERSION_MINOR 0
#define UXNAME_VERSION_PATCH 1

#include <array>
#include <cassert>
#include <cstdint>
#include <cstddef>
#include <iosfwd>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>
#include <tuple>

#if !defined(UXNAME_USING_ALIAS_STRING)
#include <string>
#endif
#if !defined(UXNAME_USING_ALIAS_STRING_VIEW)
#include <string_view>
#endif

#if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#include <cstdlib>
#endif

#if defined(__clang__)
#  pragma clang diagnostic push
#elif defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wstringop-overflow" // Missing terminating nul 'enum_name_v'.
#elif defined(_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable : 26495) // Variable 'cstring<N>::chars_' is uninitialized.
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

// Enum value must be greater or equals than UXNAME_ENUM_RANGE_MIN. By default UXNAME_ENUM_RANGE_MIN = -128.
// If need another min range for all enum types by default, redefine the macro UXNAME_ENUM_RANGE_MIN.
#if !defined(UXNAME_ENUM_RANGE_MIN)
#  define UXNAME_ENUM_RANGE_MIN -128
#endif

// Enum value must be less or equals than UXNAME_ENUM_RANGE_MAX. By default UXNAME_ENUM_RANGE_MAX = 128.
// If need another max range for all enum types by default, redefine the macro UXNAME_ENUM_RANGE_MAX.
#if !defined(UXNAME_ENUM_RANGE_MAX)
#  define UXNAME_ENUM_RANGE_MAX 128
#endif

#ifndef UBPA_TSTR
#define UBPA_TSTR

#include <utility>
#include <string_view>

namespace Ubpa {
	template<typename Char, Char... chars>
	struct TStr;
}

namespace Ubpa::details {
	template<typename STR>
	struct TSTRSizeof;
	template<typename Char>
	struct TSTRSizeof<std::basic_string_view<Char>> {
		static constexpr size_t get(const std::basic_string_view<Char>& str) noexcept {
			return str.size();
		}
	};
	template<typename Char>
	struct TSTRSizeof<const std::basic_string_view<Char>&> : TSTRSizeof<std::basic_string_view<Char>> {};
	template<typename Char>
	struct TSTRSizeof<const std::basic_string_view<Char>> : TSTRSizeof<std::basic_string_view<Char>> {};

	template<typename Char, size_t N>
	struct TSTRSizeof<const Char(&)[N]> {
		static constexpr size_t get(const Char(&str)[N]) noexcept {
			return N - 1;
		}
	};

	template <typename Char, typename T, size_t ...N>
	constexpr decltype(auto) TSTRHelperImpl(std::index_sequence<N...>) {
		return TStr<Char, T::get()[N]...>{};
	}

	template <typename T>
	constexpr decltype(auto) TSTRHelper(T) {
		using Char = std::decay_t<decltype(T::get()[0])>;
		return TSTRHelperImpl<Char, T>(std::make_index_sequence<TSTRSizeof<decltype(T::get())>::get(T::get())>());
	}
}

// [C-style string type (value)]
// in C++20, we can easily put a string into template parameter list
// but in C++17, we just can use this disgusting trick
#define TSTR(s)                                                           \
(Ubpa::details::TSTRHelper([] {                                           \
    struct tmp { static constexpr decltype(auto) get() { return (s); } }; \
    return tmp{};                                                         \
}()))

namespace Ubpa {
	template<typename Char_, Char_... chars>
	struct TStr {
		using Tag = TStr;
		using Char = Char_;
		template<typename T>
		static constexpr bool NameIs(T = {}) noexcept { return std::is_same_v<T, Tag>; }
		static constexpr char name_data[]{ chars...,Char(0) };
		static constexpr std::basic_string_view<Char> name{ name_data };
	};

	template<typename T>
	struct IsTStr : std::false_type {};

	template<typename Char, Char... chars>
	struct IsTStr<TStr<Char, chars...>> : std::true_type {};
}

#endif // UBPA_TSTR

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
	// TStr Utils
	///////////////

	template<typename Str0, typename Str1>
	struct concat_helper;
	template<typename Str0, typename Str1>
	using concat_helper_t = typename concat_helper<Str0, Str1>::type;
	template<typename Char, Char... c0, Char... c1>
	struct concat_helper<TStr<Char, c0...>, TStr<Char, c1...>> {
		using type = TStr<Char, c0..., c1...>;
	};

	template<typename Str0, typename Str1>
	constexpr auto concat(Str0 = {}, Str1 = {}) noexcept {
		return typename concat_helper<Str0, Str1>::type{};
	}

	template<typename... Strs>
	struct concat_seq_helper;
	template<typename... Strs>
	using concat_seq_helper_t = typename concat_seq_helper<Strs...>::type;
	template<typename Str>
	struct concat_seq_helper<Str> {
		using type = Str;
	};
	template<typename Str, typename... Strs>
	struct concat_seq_helper<Str, Strs...> {
		using type = concat_helper_t<Str, concat_seq_helper_t<Strs...>>;
	};
	template<typename... Strs>
	constexpr auto concat_seq(Strs...) noexcept {
		return concat_seq_helper_t<Strs...>{};
	}

	template<typename Seperator, typename... Strs>
	struct concat_seq_seperator_helper;
	template<typename Seperator, typename... Strs>
	using concat_seq_seperator_helper_t = typename concat_seq_seperator_helper<Seperator, Strs...>::type;
	template<typename Seperator>
	struct concat_seq_seperator_helper<Seperator> {
		using type = TStr<typename Seperator::Char>;
	};
	template<typename Seperator, typename Str>
	struct concat_seq_seperator_helper<Seperator, Str> {
		using type = Str;
	};
	template<typename Seperator, typename Str, typename... Strs>
	struct concat_seq_seperator_helper<Seperator, Str, Strs...> {
		using type = concat_helper_t<concat_helper_t<Str, Seperator>, concat_seq_seperator_helper_t<Seperator, Strs...>>;
	};
	template<typename Seperator, typename... Strs>
	constexpr auto concat_seq_seperator(Seperator, Strs...) noexcept {
		return concat_seq_seperator_helper_t<Seperator, Strs...>{};
	}

	template<typename Str, typename X>
	constexpr size_t find(Str = {}, X = {}) noexcept {
		static_assert(IsTStr<Str>::value && IsTStr<X>::value);
		if constexpr (Str::name.size() >= X::name.size()) {
			for (size_t i = 0; i < Str::name.size() - X::name.size() + 1; i++) {
				bool flag = true;
				for (size_t k = 0; k < X::name.size(); k++) {
					if (Str::name[i + k] != X::name[k]) {
						flag = false;
						break;
					}
				}
				if (flag)
					return i;
			}
		}
		return static_cast<size_t>(-1);
	}

	template<typename Str, typename X>
	constexpr size_t find_last(Str = {}, X = {}) noexcept {
		static_assert(IsTStr<Str>::value && IsTStr<X>::value);
		if constexpr (Str::name.size() >= X::name.size()) {
			for (size_t i = 0; i < Str::name.size() - X::name.size() + 1; i++) {
				size_t idx = Str::name.size() - X::name.size() - i;
				bool flag = true;
				for (size_t k = 0; k < X::name.size(); k++) {
					if (Str::name[idx + k] != X::name[k]) {
						flag = false;
						break;
					}
				}
				if (flag)
					return idx;
			}
		}
		return static_cast<size_t>(-1);
	}

	template<typename Str, typename X>
	constexpr bool starts_with(Str = {}, X = {}) noexcept {
		static_assert(IsTStr<Str>::value && IsTStr<X>::value);
		if (Str::name.size() < X::name.size())
			return false;
		for (size_t i = 0; i < X::name.size(); i++) {
			if (Str::name[i] != X::name[i])
				return false;
		}
		return true;
	}

	template<typename Str, typename X>
	constexpr bool ends_with(Str = {}, X = {}) noexcept {
		static_assert(IsTStr<Str>::value && IsTStr<X>::value);
		if (Str::name.size() < X::name.size())
			return false;
		for (size_t i = 0; i < X::name.size(); i++) {
			if (Str::name[Str::name.size() - X::name.size() + i] != X::name[i])
				return false;
		}
		return true;
	}

	template<size_t N, typename Str>
	constexpr auto remove_prefix(Str = {}) {
		static_assert(IsTStr<Str>::value);
		if constexpr (Str::name.size() >= N)
			return TSTR(decltype(Str::name){Str::name.data() + N});
		else
			return TSTR("");
	}

	template<typename Str, typename X>
	constexpr auto remove_prefix(Str = {}, X = {}) {
		static_assert(IsTStr<Str>::value);
		static_assert(IsTStr<X>::value);
		if constexpr (starts_with<Str, X>())
			return remove_prefix<X::name.size(), Str>();
		else
			return Str{};
	}

	template<size_t N, typename Str>
	constexpr auto remove_suffix(Str = {}) {
		static_assert(IsTStr<Str>::value);
		if constexpr (Str::name.size() >= N)
			return TSTR((decltype(Str::name){Str::name.data(), Str::name.size() - N}));
		else
			return TSTR("");
	}

	template<typename Str, typename X>
	constexpr auto remove_suffix(Str = {}, X = {}) {
		static_assert(IsTStr<Str>::value);
		if constexpr (ends_with<Str, X>())
			return remove_suffix<X::name.size(), Str>();
		else
			return Str{};
	}

	template<size_t N, typename Str>
	constexpr auto get_prefix(Str = {}) {
		static_assert(IsTStr<Str>::value);
		if constexpr (Str::name.size() >= N)
			return TSTR((decltype(Str::name){Str::name.data(), N}));
		else
			return Str{};
	}

	template<size_t N, typename Str>
	constexpr auto get_suffix(Str = {}) {
		static_assert(IsTStr<Str>::value);
		if constexpr (Str::name.size() >= N)
			return TSTR(decltype(Str::name){Str::name.data() + Str::name.size() - N});
		else
			return Str{};
	}

	// [Left, Right)
	template<size_t Idx, size_t Cnt, typename Str, typename X>
	constexpr auto replace(Str = {}, X = {}) {
		static_assert(IsTStr<Str>::value);
		static_assert(IsTStr<X>::value);
		constexpr auto prefix = remove_suffix<Str::name.size() - Idx>(Str{});
		constexpr auto suffix = remove_prefix<Idx + Cnt>(Str{});

		return concat(concat(prefix, X{}), suffix);
	}

	template<typename Str, typename From, typename To>
	constexpr auto replace(Str = {}, From = {}, To = {}) {
		static_assert(IsTStr<Str>::value);
		static_assert(IsTStr<From>::value);
		static_assert(IsTStr<To>::value);
		constexpr size_t idx = find(Str{}, From{});
		if constexpr (idx != static_cast<size_t>(-1))
			return replace(replace<idx, From::name.size()>(Str{}, To{}), From{}, To{});
		else
			return Str{};
	}

	template<typename Str, typename X>
	constexpr auto remove(Str = {}, X = {}) {
		return replace(Str{}, X{}, TSTR(""));
	}

	template<size_t Idx, size_t Cnt, typename Str>
	constexpr auto substr(Str = {}) {
		return get_prefix<Cnt>(remove_prefix<Idx, Str>());
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
		return details::remove_suffix<1>(details::remove_prefix<39>(sig));
#  elif defined(__GNUC__)
		return details::remove_suffix<1>(details::remove_prefix<54>(sig));
#  elif defined(_MSC_VER)
		return details::remove_suffix(details::remove_suffix<17>(details::remove_prefix<79>(sig)), TSTR(" "));
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
		constexpr auto idx = details::find_last(name, TSTR(":"));
		if constexpr (idx != static_cast<size_t>(-1))
			return details::remove_prefix<idx+1>(name);
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
			return details::concat_seq_seperator(TSTR(","), type_name<Ts>()...);
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
			return details::concat_seq_seperator(TSTR(","), type_name<Ts>()...);
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
			return details::concat_seq(TSTR("&{"), type_name<std::remove_reference_t<T>>(), TSTR("}"));
		else if constexpr (std::is_rvalue_reference_v<T>)
			return details::concat_seq(TSTR("&&{"), type_name<std::remove_reference_t<T>>(), TSTR("}"));
		else if constexpr (std::is_const_v<T>)
			return details::concat_seq(TSTR("const{"), type_name<std::remove_const_t<T>>(), TSTR("}"));
		else if constexpr (std::is_volatile_v<T>)
			return details::concat_seq(TSTR("volatile{"), type_name<std::remove_volatile_t<T>>(), TSTR("}"));
		else if constexpr (std::is_member_pointer_v<T>) {
			using Obj = typename member_pointer_to_object<T>::type;
			using Value = typename member_pointer_to_value<T>::type;
			return details::concat_seq(TSTR("{"), type_name<Obj>(), TSTR("}::*{"), type_name<Value>());
		}
		else if constexpr (std::is_pointer_v<T>)
			return details::concat_seq(TSTR("*{"), type_name<std::remove_pointer_t<T>>(), TSTR("}"));
		else if constexpr (std::is_array_v<T>) {
			constexpr auto r = std::rank_v<T>;
			constexpr auto ex = std::extent_v<T, 0>;
			if constexpr (ex == 0)
				return details::concat_seq(TSTR("[]{"), type_name<std::remove_extent_t<T>>(), TSTR("}"));
			else
				return details::concat_seq(TSTR("["), TSTR("CONSTANT"), TSTR("]{"), type_name<std::remove_extent_t<T>>(), TSTR("}")); // TODO
		}
		else if constexpr (std::is_function_v<T>) {
			// const volatile &/&& noexcept
			using Ret = FuncTraits_Ret<T>;
			using ArgList = FuncTraits_ArgList<T>;
			return details::concat_seq(TSTR("("), function_args_name<ArgList>(), TSTR(")->"), type_name<Ret>());
		}
		else if constexpr (details::is_integral_constant<T>::value)
			return TSTR("CONSTANT");
		else {
			using U = details::to_typename_template_type_t<T>;
			if constexpr (details::is_template_type_v<U>)
				return details::concat_seq(no_template_type_name<T>(), TSTR("<"), template_args_name<U>(), TSTR(">"));
			else
				return details::remove_class_key(raw_type_name<T>());
		}
	}
}

#if defined(__clang__)
#  pragma clang diagnostic pop
#elif defined(__GNUC__)
#  pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#  pragma warning(pop)
#endif

#endif // UBPA_UXNAME_HPP
