#pragma once

#include "../TSTR.h"
#include "../Func.h"

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
		if constexpr (Str::name.size() == 0)
			return static_cast<size_t>(-1);
		else if constexpr (Str::name.back() != '>')
			return static_cast<size_t>(-1);
		else {
			size_t k = 0;
			size_t i = Str::name.size();
			while (i > 0) {
				--i;
				if (Str::name[i] == '<') {
					k--;
					if (k == 0)
						return i;
				}
				else if (Str::name[i] == '>')
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
		return concat_seq(TSTR("volatile{"), type_name<std::remove_const_t<T>>(), TSTR("}"));
	else if constexpr (std::is_member_pointer_v<T>)
		return concat_seq(type_name<member_pointer_traits_object<T>>(), TSTR("::*{"), type_name<member_pointer_traits_value<T>>(), TSTR("}"));
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
		if constexpr (std::is_signed_v<T>) {
			constexpr size_t size = sizeof(T);
			if constexpr (size == 1)
				return TSTR("int8");
			else if constexpr (size == 2)
				return TSTR("int16");
			else if constexpr (size == 4)
				return TSTR("int32");
			else if constexpr (size == 8)
				return TSTR("int64");
			else
				static_assert("not support");
		}
		else {
			constexpr size_t size = sizeof(T);
			if constexpr (size == 1)
				return TSTR("uint8");
			else if constexpr (size == 2)
				return TSTR("uint16");
			else if constexpr (size == 4)
				return TSTR("uint32");
			else if constexpr (size == 8)
				return TSTR("uint64");
			else
				static_assert("not support");
		}
	}
#endif // UBPA_NAME_X_INT
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
