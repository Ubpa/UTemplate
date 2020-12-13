#pragma once

#include "../TSTR.h"
#include "../Func.h"

#include <cassert>

// Checks UBPA_NAME_type compiler compatibility.
#if defined(__clang__) && __clang_major__ >= 5 || defined(__GNUC__) && __GNUC__ >= 7 || defined(_MSC_VER) && _MSC_VER >= 1910
#  undef  UBPA_NAME_TYPE_SUPPORTED
#  define UBPA_NAME_TYPE_SUPPORTED 1
#endif

namespace Ubpa::details {
	//
	// core
	/////////

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
	// custom
	///////////

	template<typename Obj, typename T, T Obj::* MemPtr>
	struct member_pointer_name;

	template<auto MemPtr>
	constexpr auto member_pointer_name_v = member_pointer_name<
		member_pointer_traits_object<decltype(MemPtr)>,
		member_pointer_traits_value<decltype(MemPtr)>,
		MemPtr
	>::get();

	template<typename T> struct type_namespace_name;
	template<typename T> constexpr auto type_namespace_name_v = type_namespace_name<T>::get();

	//
	// decode
	///////////

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
	constexpr size_t get_template_idx(Str = {}) {
		if constexpr (Str::value.size() == 0)
			return static_cast<size_t>(-1);
		else if constexpr (Str::value.back() != '>')
			return static_cast<size_t>(-1);
		else {
			size_t k = 0;
			size_t i = Str::value.size();
			while (i > 0) {
				--i;
				if (Str::value[i] == '<') {
					k--;
					if (k == 0)
						return i;
				}
				else if (Str::value[i] == '>')
					k++;
			}
			return static_cast<size_t>(-1);
		}
	}

	template<typename Str>
	constexpr auto remove_template(Str = {}) {
		constexpr auto idx = get_template_idx<Str>();
		if constexpr (idx != static_cast<size_t>(-1))
			return substr<0, idx, Str>();
		else
			return Str{};
	}

	template<typename T>
	constexpr auto raw_type_name() noexcept {
		constexpr auto sig = func_signature<T>();
#if defined(UBPA_NAME_TYPE_SUPPORTED) && UBPA_NAME_TYPE_SUPPORTED
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
	constexpr auto no_template_type_name() noexcept {
		constexpr auto name0 = raw_type_name<T>();
		constexpr auto name1 = remove_class_key(name0);
		constexpr auto name2 = remove_template(name1);
		constexpr auto idx = find_last(name2, TSTR(":"));
		if constexpr (idx != static_cast<size_t>(-1) && is_defined_v<type_namespace_name<T>>)
			return concat_seq(type_namespace_name_v<T>, TSTR("::"), remove_prefix<idx + 1>(name2));
		else
			return name2;
	}

	template<typename T>
	struct template_args_name_impl;
	template<template<typename...>class T, typename... Ts>
	struct template_args_name_impl<T<Ts...>> {
		constexpr static auto get() noexcept {
			return concat_seq_seperator(TSTR(","),
				concat_seq(TSTR("{"), type_name<Ts>(), TSTR("}"))...);
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
			return concat_seq_seperator(TSTR(","),
				concat_seq(TSTR("{"), type_name<Ts>(), TSTR("}"))...);
		}
	};

	template<typename T>
	constexpr auto function_args_name() noexcept {
		return function_args_name_impl<T>::get();
	}

	constexpr bool is_start_with(std::string_view str, std::string_view prefix) noexcept {
		if (str.size() < prefix.size())
			return false;

		for (size_t i = 0; i < prefix.size(); i++) {
			if (str[i] != prefix[i])
				return false;
		}

		return true;
	}
	template<size_t N>
	constexpr bool is_start_with(std::string_view str, const char(&prefix)[N]) noexcept {
		if (str.size() < N - 1)
			return false;

		for (size_t i = 0; i < N - 1; i++) {
			if (str[i] != prefix[i])
				return false;
		}

		return true;
	}
}

template<auto V>
constexpr auto Ubpa::constexpr_name() noexcept {
	using T = decltype(V);
	if constexpr (std::is_null_pointer_v<T>)
		return TSTR("nullptr");
	else if constexpr (std::is_pointer_v<T>) {
		if constexpr (V == nullptr)
			return TSTR("nullptr");
		else
			static_assert("not support");
	}
	else if constexpr (std::is_member_pointer_v<T>) {
		if constexpr (V == nullptr)
			return TSTR("nullptr");
		else {
			using Object = member_pointer_traits_object<T>;
			using Value = member_pointer_traits_value<T>;
			if constexpr (is_defined_v<details::member_pointer_name<Object, Value, V>>) {
				return concat_seq(TSTR("&"), type_name<Object>(), TSTR("::"), details::member_pointer_name_v<V>);
			}
			else
				return concat_seq(TSTR("&"), type_name<Object>(), TSTR("::#UNKNOWN"));
		}
	}
	else if constexpr (std::is_integral_v<T>) {
		if constexpr (std::is_same_v<T, bool>) {
			if constexpr (V == true)
				return TSTR("true");
			else
				return TSTR("false");
		}
		else
			return int_to_TSTR<V>();
	}
	else
		static_assert("not support");
}

template<typename T>
constexpr auto Ubpa::type_name() noexcept {
	if constexpr (std::is_lvalue_reference_v<T>)
		return concat_seq(TSTR("&{"), type_name<std::remove_reference_t<T>>(), TSTR("}"));
	else if constexpr (std::is_rvalue_reference_v<T>)
		return concat_seq(TSTR("&&{"), type_name<std::remove_reference_t<T>>(), TSTR("}"));
	else if constexpr (std::is_const_v<T> && std::is_volatile_v<T>)
		return concat_seq(TSTR("const volatile{"), type_name<std::remove_cv_t<T>>(), TSTR("}"));
	else if constexpr (std::is_const_v<T> && !std::is_volatile_v<T>)
		return concat_seq(TSTR("const{"), type_name<std::remove_const_t<T>>(), TSTR("}"));
	else if constexpr (!std::is_const_v<T> && std::is_volatile_v<T>)
		return concat_seq(TSTR("volatile{"), type_name<std::remove_volatile_t<T>>(), TSTR("}"));
	else if constexpr (std::is_member_pointer_v<T>)
		return concat_seq(TSTR("{"), type_name<member_pointer_traits_object<T>>(), TSTR("}::*{"), type_name<member_pointer_traits_value<T>>(), TSTR("}"));
	else if constexpr (std::is_pointer_v<T>)
		return concat_seq(TSTR("*{"), type_name<std::remove_pointer_t<T>>(), TSTR("}"));
	else if constexpr (std::is_array_v<T>) {
		constexpr auto r = std::rank_v<T>;
		constexpr auto ex = std::extent_v<T, 0>;
		if constexpr (r == 1) {
			if constexpr (ex == 0)
				return concat_seq(TSTR("[]{"), type_name<std::remove_extent_t<T>>(), TSTR("}"));
			else
				return concat_seq(TSTR("["), constexpr_name<ex>(), TSTR("]{"), type_name<std::remove_extent_t<T>>(), TSTR("}"));
		}
		else { // r > 1
			static_assert(r > 1);
			if constexpr (ex == 0)
				return concat_seq(TSTR("[]"), type_name<std::remove_extent_t<T>>());
			else
				return concat_seq(TSTR("["), constexpr_name<ex>(), TSTR("]"), type_name<std::remove_extent_t<T>>());
		}
	}
	else if constexpr (std::is_function_v<T>) {
		using Traits = FuncTraits<T>;
		using Return = FuncTraits_Return<T>;
		using ArgList = FuncTraits_ArgList<T>;
		constexpr auto ArgsName = concat_seq(TSTR("("), details::function_args_name<ArgList>(), TSTR(")"));
		constexpr auto RetName = concat_seq(TSTR("{"), type_name<Return>(), TSTR("}"));
		// const, volatile, &/&&, noexcept : 24
		if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::NONE && !Traits::is_noexcept) // 0000
			return concat_seq(ArgsName, TSTR("-->"), RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::NONE && !Traits::is_noexcept) // 1000
			return concat_seq(ArgsName, TSTR("-{const}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::NONE && !Traits::is_noexcept) // 0100
			return concat_seq(ArgsName, TSTR("-{volatile}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::NONE && !Traits::is_noexcept) // 1100
			return concat_seq(ArgsName, TSTR("-{const volatile}->"), RetName);
		else if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::LEFT && !Traits::is_noexcept) // 0010
			return concat_seq(ArgsName, TSTR("-{&}->"), RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::LEFT && !Traits::is_noexcept) // 1010
			return concat_seq(ArgsName, TSTR("-{const &}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::LEFT && !Traits::is_noexcept) // 0110
			return concat_seq(ArgsName, TSTR("-{volatile &}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::RIGHT && !Traits::is_noexcept) // 1110
			return concat_seq(ArgsName, TSTR("-{const volatile &}->"), RetName);
		else if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::RIGHT && !Traits::is_noexcept) // 0020
			return concat_seq(ArgsName, TSTR("-{&&}->"), RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::RIGHT && !Traits::is_noexcept) // 1020
			return concat_seq(ArgsName, TSTR("-{const &&}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::RIGHT && !Traits::is_noexcept) // 0120
			return concat_seq(ArgsName, TSTR("-{volatile &&}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::RIGHT && !Traits::is_noexcept) // 1120
			return concat_seq(ArgsName, TSTR("-{const volatile &&}->"), RetName);
		else if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::NONE && Traits::is_noexcept) // 0001
			return concat_seq(ArgsName, TSTR("-{noexcept}->"), RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::NONE && Traits::is_noexcept) // 1001
			return concat_seq(ArgsName, TSTR("-{const noexcept}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::NONE && Traits::is_noexcept) // 0101
			return concat_seq(ArgsName, TSTR("-{volatile noexcept}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::NONE && Traits::is_noexcept) // 1101
			return concat_seq(ArgsName, TSTR("-{const volatile noexcept}->"), RetName);
		else if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::LEFT && Traits::is_noexcept) // 0011
			return concat_seq(ArgsName, TSTR("-{& noexcept}->"), RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::LEFT && Traits::is_noexcept) // 1011
			return concat_seq(ArgsName, TSTR("-{const & noexcept}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::LEFT && Traits::is_noexcept) // 0111
			return concat_seq(ArgsName, TSTR("-{volatile & noexcept}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::RIGHT && Traits::is_noexcept) // 1111
			return concat_seq(ArgsName, TSTR("-{const volatile & noexcept}->"), RetName);
		else if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::RIGHT && Traits::is_noexcept) // 0021
			return concat_seq(ArgsName, TSTR("-{&& noexcept}->"), RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::RIGHT && Traits::is_noexcept) // 1021
			return concat_seq(ArgsName, TSTR("-{const && noexcept}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::RIGHT && Traits::is_noexcept) // 0121
			return concat_seq(ArgsName, TSTR("-{volatile && noexcept}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::RIGHT && Traits::is_noexcept) // 1121
			return concat_seq(ArgsName, TSTR("-{const volatile && noexcept}->"), RetName);
		else
			static_assert("not support");
	}
	else if constexpr (IsIValue_v<T>)
		return constexpr_name<T::value>();
#ifdef UBPA_NAME_X_INT
	else if constexpr (std::is_integral_v<T>) {
		static_assert(sizeof(T) <= 8);
		constexpr auto BitName = constexpr_name<8 * sizeof(T)>();
		if constexpr (std::is_signed_v<T>)
			return concat(TSTR("int"), BitName);
		else
			return concat(TSTR("uint"), BitName);
	}
#endif // UBPA_NAME_X_INT
#ifdef UBPA_NAME_X_FLOAT
	else if constexpr (std::is_floating_point_v<T>)
		return concat(TSTR("float"), constexpr_name<8 * sizeof(T)>());
#endif // UBPA_NAME_X_FLOAT
	else {
		using U = to_typename_template_type_t<T>;
		if constexpr (is_typename_template_type_v<U>)
			return concat_seq(details::no_template_type_name<T>(), TSTR("<"), details::template_args_name<U>(), TSTR(">"));
		else {
			constexpr auto name = details::remove_class_key(details::raw_type_name<T>());
			constexpr auto idx = find_last(name, TSTR(":"));
			if constexpr (idx != static_cast<size_t>(-1) && is_defined_v<details::type_namespace_name<T>>)
				return concat_seq(details::type_namespace_name_v<T>, TSTR("::"), remove_prefix<idx + 1>(name));
			else
				return name;
		}
	}
}

constexpr bool Ubpa::constexpr_name_is_null_pointer(std::string_view name) noexcept {
	return name == constexpr_name<nullptr>().value;
}

constexpr bool Ubpa::constexpr_name_is_integral(std::string_view name) noexcept {
	if (name.empty())
		return false;

	for (size_t i = name.front() == '-' ? 1 : 0; i < name.size(); i++) {
		if (name[i] < '0' || name[i] > '9')
			return false;
	}

	return true;
}

constexpr bool Ubpa::type_name_is_void(std::string_view name) noexcept {
	return name == type_name<void>().value;
}

constexpr bool Ubpa::type_name_is_null_pointer(std::string_view name) noexcept {
	return type_name_remove_cv(name) == type_name<nullptr_t>().value;
}

constexpr bool Ubpa::type_name_is_integral(std::string_view name) noexcept {
	switch (string_hash(type_name_remove_cv(name)))
	{
	case string_hash(type_name<int8_t>().value):
	case string_hash(type_name<int16_t>().value):
	case string_hash(type_name<int32_t>().value):
	case string_hash(type_name<int64_t>().value):
	case string_hash(type_name<uint8_t>().value):
	case string_hash(type_name<uint16_t>().value):
	case string_hash(type_name<uint32_t>().value):
	case string_hash(type_name<uint64_t>().value):
		return true;
	default:
		return false;
	}
}

constexpr bool Ubpa::type_name_is_floating_point(std::string_view name) noexcept {
	auto rmcv_name = type_name_remove_cv(name);
	if (rmcv_name == type_name<float>().value)
		return true;
	else if (rmcv_name == type_name<double>().value)
		return true;
	else {
		if constexpr (type_name<double>().value == type_name<long double>().value)
			return false;
		else {
			return rmcv_name == type_name<long double>().value;
		}
	}
}

constexpr bool Ubpa::type_name_is_array(std::string_view name) noexcept {
	return details::is_start_with(name, "[");
}

constexpr bool Ubpa::type_name_is_function(std::string_view name) noexcept {
	return details::is_start_with(name, "(");
}

constexpr bool Ubpa::type_name_is_pointer(std::string_view name) noexcept {
	return details::is_start_with(name, "*");
}

constexpr bool Ubpa::type_name_is_lvalue_reference(std::string_view name) noexcept {
	return details::is_start_with(name, "&{");
}

constexpr bool Ubpa::type_name_is_rvalue_reference(std::string_view name) noexcept {
	return details::is_start_with(name, "&&");
}

constexpr bool Ubpa::type_name_is_member_pointer(std::string_view name) noexcept {
	return details::is_start_with(name, "{");
}

// properties

constexpr bool Ubpa::type_name_is_const(std::string_view name) noexcept {
	return details::is_start_with(name, "const{") || details::is_start_with(name, "const ");
}

constexpr bool Ubpa::type_name_is_volatile(std::string_view name) noexcept {
	return details::is_start_with(name, "volatile{") || details::is_start_with(name, "const volatile");
}

constexpr bool Ubpa::type_name_is_cv(std::string_view name) noexcept {
	return details::is_start_with(name, "const volatile");
}


constexpr bool Ubpa::type_name_is_signed(std::string_view name) noexcept {
	return !type_name_is_unsigned(name);
}

constexpr bool Ubpa::type_name_is_unsigned(std::string_view name) noexcept {
	switch (string_hash(name))
	{
	case string_hash(type_name<uint8_t>().value):
	case string_hash(type_name<uint16_t>().value):
	case string_hash(type_name<uint32_t>().value):
	case string_hash(type_name<uint64_t>().value):
		return true;
	default:
		return false;
	}
}

constexpr bool Ubpa::type_name_is_bounded_array(std::string_view name) noexcept {
	if (name.size() < 2)
		return false;

	return name[0] == '[' && name[1] != ']';
}

constexpr bool Ubpa::type_name_is_unbounded_array(std::string_view name) noexcept {
	return details::is_start_with(name, "[]");
}

constexpr size_t Ubpa::type_name_rank(std::string_view name) noexcept {
	size_t rank = 0;
	size_t idx = 0;
	bool flag = false;
	while (idx < name.size()) {
		if (!flag) {
			if (name[idx] == '[')
				++rank;
			else
				return rank;
			flag = true;
		}
		else {
			if (name[idx] == ']')
				flag = false;
		}
		++idx;
	}
	return rank;
}

constexpr size_t Ubpa::type_name_extent(std::string_view name, size_t N) noexcept {
	size_t idx = 0;
	while (N != 0) {
		if (name[idx] != '[')
			return false;
		++idx;
		while (name[idx++] != ']')
			assert(idx < name.size());
		--N;
	}

	assert(idx < name.size());

	if (name[idx] != '[')
		return 0;

	size_t extent = 0;
	while (name[++idx] != ']') {
		assert(idx < name.size());
		extent = 10 * extent + name[idx] - '0';
	}

	return extent;
}

// modification (clip)

constexpr std::string_view Ubpa::type_name_remove_cv(std::string_view name) noexcept {
	if (details::is_start_with(name, "const")) {
		assert(name.size() >= 6);
		if (name[5] == '{') {
			assert(*name.rbegin() == '}');
			return { name.data() + 6,name.size() - 7 };
		}
		else if (name[5] == ' ') {
			assert(details::is_start_with(name, "const volatile{") && *name.rbegin() == '}');
			return { name.data() + 15,name.size() - 16 };
		}
		else
			return name;
	}
	else if (details::is_start_with(name, "volatile{")) {
		assert(*name.rbegin() == '}');
		return { name.data() + 9,name.size() - 10 };
	}
	else
		return name;
}

constexpr std::string_view Ubpa::type_name_remove_const(std::string_view name) noexcept {
	if (!details::is_start_with(name, "const"))
		return name;

	assert(name.size() >= 6);

	if (name[5] == '{') {
		assert(*name.rbegin() == '}');
		return { name.data() + 6,name.size() - 7 };
	}
	else if (name[5] == ' ')
		return { name.data() + 6 };
	else
		return name;
}

constexpr std::string_view Ubpa::type_name_remove_topmost_volatile(std::string_view name) noexcept {
	if (!details::is_start_with(name, "volatile{"))
		return name;

	assert(*name.rbegin() == '}');

	return { name.data() + 9,name.size() - 10 };
}

constexpr std::string_view Ubpa::type_name_remove_reference(std::string_view name) noexcept {
	if (!details::is_start_with(name, "&"))
		return name;

	assert(name.size() >= 2);
	if (name[1] == '{') {
		assert(*name.rbegin() == '}');
		return { name.data() + 2, name.size() - 3 };
	}
	else {
		assert(name.size() >= 3 && name[1] == '&' && name[2] == '{' && *name.rbegin() == '}');
		return { name.data() + 3, name.size() - 4 };
	}
}

constexpr std::string_view Ubpa::type_name_remove_pointer(std::string_view name) noexcept {
	name = type_name_remove_cvref(name);
	if (!details::is_start_with(name, "*"))
		return name;

	assert(name.size() >= 3 && name[1] == '{' && *name.rbegin() == '}');
	return { name.data() + 2, name.size() - 3 };
}

constexpr std::string_view Ubpa::type_name_remove_cvref(std::string_view name) noexcept {
	return type_name_remove_cv(type_name_remove_reference(name));
}

constexpr std::string_view Ubpa::type_name_remove_extent(std::string_view name) noexcept {
	size_t idx = 0;

	if (name.empty())
		return name;

	if (name[idx] != '[')
		return name;
	
	++idx;
	while (name[idx++] != ']')
		assert(idx < name.size());

	assert(name.size() > idx);

	if (name[idx] == '[')
		return { name.data() + idx, name.size() - idx };
	else {
		assert(name[idx] == '{' && *name.rbegin() == '}');
		return { name.data() + idx + 1, name.size() - idx - 2 };
	}
}

constexpr std::string_view Ubpa::type_name_remove_all_extents(std::string_view name) noexcept {
	if (!type_name_is_array(name))
		return name;

	return type_name_remove_all_extents(type_name_remove_extent(name));
}

// composite

constexpr bool Ubpa::type_name_is_arithmetic(std::string_view name) noexcept {
	return type_name_is_integral(name) || type_name_is_floating_point(name);
}

constexpr bool Ubpa::type_name_is_fundamental(std::string_view name) noexcept {
	return type_name_is_arithmetic(name)
		|| type_name_is_void(name)
		|| type_name_is_null_pointer(name)
		;
}