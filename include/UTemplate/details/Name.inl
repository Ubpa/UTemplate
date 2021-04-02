#pragma once

#include "../TStr.hpp"
#include "../Func.hpp"

#include <cassert>
#include <cstring>

namespace Ubpa::details {
	//
	// core
	/////////

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
		return TSTR(func_signature_impl<T>());
	}

	//
	// custom
	///////////

	template<typename T>
	struct custom_type_name;

	template<auto Value>
	struct custom_constexpr_value_name;

	template<auto Value>
	constexpr auto custom_constexpr_value_name_v = custom_constexpr_value_name<Value>::get();

	template<typename T> struct custom_type_namespace_name;
	template<typename T> constexpr auto custom_type_namespace_name_v = custom_type_namespace_name<T>::get();

	//
	// decode
	///////////

	template<typename T>
	constexpr auto raw_type_name() noexcept {
		constexpr auto sig = func_signature<T>();
#if defined(__clang__)
		return remove_suffix<1>(remove_prefix<47>(sig));
#elif defined(__GNUC__)
		return remove_suffix<1>(remove_prefix<62>(sig));
#elif defined(_MSC_VER)
		return remove_suffix(remove_suffix<16>(remove_prefix<48>(sig)), TStr_of_a<' '>{});
#endif
	}

	template<typename Str>
	constexpr auto remove_class_key(Str = {}) {
#if defined(__clang__)
		return Str{};
#elif defined(__GNUC__)
		return Str{};
#elif defined(_MSC_VER)
		constexpr auto n0 = remove_prefix(Str{}, TStrC_of<'s','t','r','u','c','t',' '>{});
		constexpr auto n1 = remove_prefix(n0, TStrC_of<'e','n','u','m',' '>{});
		constexpr auto n2 = remove_prefix(n1, TStrC_of<'c','l','a','s','s',' '>{});
		constexpr auto n3 = remove_prefix(n2, TStrC_of<'u', 'n', 'i', 'o', 'n', ' '>{});
		return n3;
#endif
	}

	template<typename Str>
	constexpr std::size_t get_template_idx(Str = {}) {
		if constexpr (Str::Size() == 0)
			return static_cast<std::size_t>(-1);
		else if constexpr (Str::View().back() != '>')
			return static_cast<std::size_t>(-1);
		else {
			std::size_t k = 0;
			std::size_t i = Str::Size();
			while (i > 0) {
				--i;
				if (Str::View()[i] == '<') {
					k--;
					if (k == 0)
						return i;
				}
				else if (Str::View()[i] == '>')
					k++;
			}
			return static_cast<std::size_t>(-1);
		}
	}

	template<typename Str>
	constexpr auto remove_template(Str = {}) {
		constexpr auto idx = get_template_idx<Str>();
		if constexpr (idx != static_cast<std::size_t>(-1))
			return substr<0, idx, Str>();
		else
			return Str{};
	}

	template<typename T>
	constexpr auto no_template_type_name() noexcept {
		constexpr auto name0 = raw_type_name<T>();
		constexpr auto name1 = remove_class_key(name0);
		constexpr auto name2 = remove_template(name1);
		constexpr auto idx = find_last(name2, TStr_of_a<':'>{});
		if constexpr (idx != static_cast<std::size_t>(-1) && is_defined_v<custom_type_namespace_name<T>>)
			return concat_seq(custom_type_namespace_name_v<T>, TStrC_of<':',':'>{}, remove_prefix<idx + 1>(name2));
		else
			return name2;
	}

	template<typename T>
	struct template_args_name_impl;
	template<template<typename...>class T, typename... Ts>
	struct template_args_name_impl<T<Ts...>> {
		constexpr static auto get() noexcept {
			return concat_seq_seperator(TStr_of_a<','>{},
				concat_seq(TStr_of_a<'{'>{}, type_name<Ts>(), TStr_of_a<'}'>{})...);
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
			return concat_seq_seperator(TStr_of_a<','>{},
				concat_seq(TStr_of_a<'{'>{}, type_name<Ts>(), TStr_of_a<'}'>{})...);
		}
	};

	template<typename T>
	constexpr auto function_args_name() noexcept {
		return function_args_name_impl<T>::get();
	}
}

template<auto V>
constexpr auto Ubpa::constexpr_value_name() noexcept {
	using T = decltype(V);
	if constexpr (std::is_null_pointer_v<T>)
		return TStrC_of<'n','u','l','l','p','t','r'>{};
	else if constexpr (std::is_pointer_v<T>) {
		if constexpr (V == nullptr)
			return TStrC_of<'n','u','l','l','p','t','r'>{};
		else
			static_assert("not support");
	}
	else if constexpr (std::is_member_pointer_v<T>) {
		if constexpr (V == nullptr)
			return TStrC_of<'n','u','l','l','p','t','r'>{};
		else {
			using Object = member_pointer_traits_object<T>;
			using Value = member_pointer_traits_value<T>;
			if constexpr (is_defined_v<details::custom_constexpr_value_name<V>>) {
				return concat_seq(TStr_of_a<'&'>{}, type_name<Object>(), TStrC_of<':',':'>{}, details::custom_constexpr_value_name_v<V>);
			}
			else
				return concat_seq(TStr_of_a<'&'>{}, type_name<Object>(), TSTR("::#UNKNOWN"));
		}
	}
	else if constexpr (std::is_integral_v<T>) {
		if constexpr (std::is_same_v<T, bool>) {
			if constexpr (V == true)
				return TStrC_of<'t','r','u','e'>{};
			else
				return TStrC_of<'f','a','l','s','e'>{};
		}
		else
			return int_to_TSTR<V>();
	}
	else
		static_assert("not support");
}

template<typename T>
constexpr auto Ubpa::type_name() noexcept {
	if constexpr (is_defined_v<details::custom_type_name<T>>)
		return details::custom_type_name<T>::get();
	else if constexpr (std::is_lvalue_reference_v<T>)
		return concat_seq(TStrC_of<'&', '{'>{}, type_name<std::remove_reference_t<T>>(), TStr_of_a<'}'>{});
	else if constexpr (std::is_rvalue_reference_v<T>)
		return concat_seq(TStrC_of<'&', '&', '{'>{}, type_name<std::remove_reference_t<T>>(), TStr_of_a<'}'>{});
	else if constexpr (std::is_const_v<T> && std::is_volatile_v<T>)
		return concat_seq(TSTR("const volatile{"), type_name<std::remove_cv_t<T>>(), TStr_of_a<'}'>{});
	else if constexpr (std::is_const_v<T> && !std::is_volatile_v<T>)
		return concat_seq(TStrC_of<'c', 'o', 'n', 's', 't', '{'>{}, type_name<std::remove_const_t<T>>(), TStr_of_a<'}'>{});
	else if constexpr (!std::is_const_v<T> && std::is_volatile_v<T>)
		return concat_seq(TSTR("volatile{"), type_name<std::remove_volatile_t<T>>(), TStr_of_a<'}'>{});
	else if constexpr (std::is_member_pointer_v<T>)
		return concat_seq(TStr_of_a<'{'>{}, type_name<member_pointer_traits_object<T>>(), TStrC_of<'}', ':', ':', '*', '{'>{}, type_name<member_pointer_traits_value<T>>(), TStr_of_a<'}'>{});
	else if constexpr (std::is_pointer_v<T>)
		return concat_seq(TStrC_of<'*', '{'>{}, type_name<std::remove_pointer_t<T>>(), TStr_of_a<'}'>{});
	else if constexpr (std::is_array_v<T>) {
		constexpr auto r = std::rank_v<T>;
		constexpr auto ex = std::extent_v<T, 0>;
		if constexpr (r == 1) {
			if constexpr (ex == 0)
				return concat_seq(TStrC_of<'[', ']', '{'>{}, type_name<std::remove_extent_t<T>>(), TStr_of_a<'}'>{});
			else
				return concat_seq(TStr_of_a<'['>{}, constexpr_value_name<ex>(), TStrC_of<']', '{'>{}, type_name<std::remove_extent_t<T>>(), TStr_of_a<'}'>{});
		}
		else { // r > 1
			static_assert(r > 1);
			if constexpr (ex == 0)
				return concat_seq(TStrC_of<'[', ']'>{}, type_name<std::remove_extent_t<T>>());
			else
				return concat_seq(TStr_of_a<'['>{}, constexpr_value_name<ex>(), TStr_of_a<']'>{}, type_name<std::remove_extent_t<T>>());
		}
	}
	else if constexpr (std::is_function_v<T>) {
		using Traits = FuncTraits<T>;
		using Return = FuncTraits_Return<T>;
		using ArgList = FuncTraits_ArgList<T>;
		constexpr auto ArgsName = concat_seq(TStr_of_a<'('>{}, details::function_args_name<ArgList>(), TStr_of_a<')'>{});
		constexpr auto RetName = concat_seq(TStr_of_a<'{'>{}, type_name<Return>(), TStr_of_a<'}'>{});
		// const, volatile, &/&&, noexcept : 24
		if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::None && !Traits::is_noexcept) // 0000
			return concat_seq(ArgsName, TStrC_of<'-', '-', '>', '{', '}'>{}, RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::None && !Traits::is_noexcept) // 1000
			return concat_seq(ArgsName, TSTR("-{const}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::None && !Traits::is_noexcept) // 0100
			return concat_seq(ArgsName, TSTR("-{volatile}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::None && !Traits::is_noexcept) // 1100
			return concat_seq(ArgsName, TSTR("-{const volatile}->"), RetName);
		else if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::Left && !Traits::is_noexcept) // 0010
			return concat_seq(ArgsName, TStrC_of<'-', '{', '&', '}', '-', '>', '{', '}'>{}, RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::Left && !Traits::is_noexcept) // 1010
			return concat_seq(ArgsName, TSTR("-{const &}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::Left && !Traits::is_noexcept) // 0110
			return concat_seq(ArgsName, TSTR("-{volatile &}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::Right && !Traits::is_noexcept) // 1110
			return concat_seq(ArgsName, TSTR("-{const volatile &}->"), RetName);
		else if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::Right && !Traits::is_noexcept) // 0020
			return concat_seq(ArgsName, TStrC_of<'-', '{', '&', '&', '}', '-', '>', '{', '}'>{}, RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::Right && !Traits::is_noexcept) // 1020
			return concat_seq(ArgsName, TSTR("-{const &&}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::Right && !Traits::is_noexcept) // 0120
			return concat_seq(ArgsName, TSTR("-{volatile &&}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::Right && !Traits::is_noexcept) // 1120
			return concat_seq(ArgsName, TSTR("-{const volatile &&}->"), RetName);
		else if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::None && Traits::is_noexcept) // 0001
			return concat_seq(ArgsName, TSTR("-{noexcept}->"), RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::None && Traits::is_noexcept) // 1001
			return concat_seq(ArgsName, TSTR("-{const noexcept}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::None && Traits::is_noexcept) // 0101
			return concat_seq(ArgsName, TSTR("-{volatile noexcept}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::None && Traits::is_noexcept) // 1101
			return concat_seq(ArgsName, TSTR("-{const volatile noexcept}->"), RetName);
		else if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::Left && Traits::is_noexcept) // 0011
			return concat_seq(ArgsName, TSTR("-{& noexcept}->"), RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::Left && Traits::is_noexcept) // 1011
			return concat_seq(ArgsName, TSTR("-{const & noexcept}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::Left && Traits::is_noexcept) // 0111
			return concat_seq(ArgsName, TSTR("-{volatile & noexcept}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::Right && Traits::is_noexcept) // 1111
			return concat_seq(ArgsName, TSTR("-{const volatile & noexcept}->"), RetName);
		else if constexpr (!Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::Right && Traits::is_noexcept) // 0021
			return concat_seq(ArgsName, TSTR("-{&& noexcept}->"), RetName);
		else if constexpr (Traits::is_const && !Traits::is_volatile && Traits::ref == ReferenceMode::Right && Traits::is_noexcept) // 1021
			return concat_seq(ArgsName, TSTR("-{const && noexcept}->"), RetName);
		else if constexpr (!Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::Right && Traits::is_noexcept) // 0121
			return concat_seq(ArgsName, TSTR("-{volatile && noexcept}->"), RetName);
		else if constexpr (Traits::is_const && Traits::is_volatile && Traits::ref == ReferenceMode::Right && Traits::is_noexcept) // 1121
			return concat_seq(ArgsName, TSTR("-{const volatile && noexcept}->"), RetName);
		else
			static_assert("not support");
	}
	else if constexpr (IsIValue_v<T>)
		return constexpr_value_name<T::value>();
	else if constexpr (std::is_same_v<T, bool>)
		return TStrC_of<'b', 'o', 'o', 'l'>{};
	else if constexpr (std::is_integral_v<T>) {
		static_assert(sizeof(T) <= 8);
		constexpr auto BitName = constexpr_value_name<8 * sizeof(T)>();
		if constexpr (std::is_signed_v<T>)
			return concat(TStrC_of<'i','n','t'>{}, BitName);
		else
			return concat(TStrC_of<'u','i','n','t'>{}, BitName);
	}
	else if constexpr (std::is_floating_point_v<T>)
		return concat(TStrC_of<'f','l','o','a','t'>{}, constexpr_value_name<8 * sizeof(T)>());
	else if constexpr (std::is_same_v<T, void>)
		return TStrC_of<'v', 'o', 'i', 'd'>{};
	else if constexpr (std::is_same_v<T, std::nullptr_t>)
		return TSTR("std::nullptr_t");
	else if constexpr (TStrLike<T>)
		return concat_seq(TSTR("TSTR<\""), TSTR(T::View()), TSTR("\">"));
	else if constexpr (std::is_enum_v<T>) {
		constexpr auto name = details::remove_class_key(details::raw_type_name<T>());
		return concat_seq(TStrC_of<'e', 'n', 'u', 'm', '{'>{}, name, TStr_of_a<'}'>{});
	}
	else if constexpr (std::is_union_v<T>) {
		constexpr auto name = details::remove_class_key(details::raw_type_name<T>());
		return concat_seq(TStrC_of<'u', 'n', 'i', 'o', 'n', '{'>{}, name, TStr_of_a<'}'>{});
	}
	else {
		using U = to_typename_template_type_t<T>;
		if constexpr (is_typename_template_type_v<U>)
			return concat_seq(details::no_template_type_name<T>(), TStr_of_a<'<'>{}, details::template_args_name<U>(), TStrC_of<'>'>{});
		else {
			constexpr auto name = details::remove_class_key(details::raw_type_name<T>());
			constexpr auto idx = find_last(name, TStr_of_a<':'>{});
			if constexpr (idx != static_cast<std::size_t>(-1) && is_defined_v<details::custom_type_namespace_name<T>>)
				return concat_seq(details::custom_type_namespace_name_v<T>, TStrC_of<':',':'>{}, remove_prefix<idx + 1>(name));
			else
				return name;
		}
	}
}

constexpr bool Ubpa::constexpr_name_is_null_pointer(std::string_view name) noexcept {
	return name == constexpr_value_name<nullptr>().View();
}

constexpr bool Ubpa::constexpr_name_is_integral(std::string_view name) noexcept {
	if (name.empty())
		return false;

	for (std::size_t i = name.front() == '-' ? 1 : 0; i < name.size(); i++) {
		if (name[i] < '0' || name[i] > '9')
			return false;
	}

	return true;
}

constexpr bool Ubpa::type_name_is_void(std::string_view name) noexcept {
	return type_name_remove_cv(name) == type_name<void>().View();
}

constexpr bool Ubpa::type_name_is_null_pointer(std::string_view name) noexcept {
	return type_name_remove_cv(name) == type_name<std::nullptr_t>().View();
}

constexpr bool Ubpa::type_name_is_integral(std::string_view name) noexcept {
	switch (string_hash(type_name_remove_cv(name)))
	{
	case string_hash(type_name<bool>().View()):
	case string_hash(type_name<int8_t>().View()):
	case string_hash(type_name<int16_t>().View()):
	case string_hash(type_name<int32_t>().View()):
	case string_hash(type_name<int64_t>().View()):
	case string_hash(type_name<uint8_t>().View()):
	case string_hash(type_name<uint16_t>().View()):
	case string_hash(type_name<uint32_t>().View()):
	case string_hash(type_name<uint64_t>().View()):
		return true;
	default:
		return false;
	}
}

constexpr bool Ubpa::type_name_is_floating_point(std::string_view name) noexcept {
	auto rmcv_name = type_name_remove_cv(name);
	if (rmcv_name == type_name<float>().View())
		return true;
	else if (rmcv_name == type_name<double>().View())
		return true;
	else {
		if constexpr (std::is_same_v<decltype(type_name<double>()), decltype(type_name<long double>())>)
			return false;
		else
			return rmcv_name == type_name<long double>().View();
	}
}

constexpr bool Ubpa::type_name_is_array(std::string_view name) noexcept {
	return name.starts_with(std::string_view{"["});
}

constexpr bool Ubpa::type_name_is_enum(std::string_view name) noexcept {
	return name.starts_with(std::string_view{"enum{"});
}

constexpr bool Ubpa::type_name_is_union(std::string_view name) noexcept {
	return name.starts_with(std::string_view{ "union{" });
}

constexpr bool Ubpa::type_name_is_function(std::string_view name) noexcept {
	return name.starts_with(std::string_view{"("});
}

constexpr bool Ubpa::type_name_is_pointer(std::string_view name) noexcept {
	return name.starts_with(std::string_view{"*"});
}

constexpr bool Ubpa::type_name_is_lvalue_reference(std::string_view name) noexcept {
	return name.starts_with(std::string_view{"&{"});
}

constexpr bool Ubpa::type_name_is_rvalue_reference(std::string_view name) noexcept {
	return name.starts_with(std::string_view{"&&"});
}

constexpr bool Ubpa::type_name_is_member_pointer(std::string_view name) noexcept {
	return name.starts_with(std::string_view{"{"});
}

// composite

constexpr bool Ubpa::type_name_is_arithmetic(std::string_view name) noexcept {
	const std::size_t noncv_name_hash = string_hash(type_name_remove_cv(name));
	switch (noncv_name_hash)
	{
	case string_hash(type_name<bool>().View()):
	case string_hash(type_name<int8_t>().View()):
	case string_hash(type_name<int16_t>().View()):
	case string_hash(type_name<int32_t>().View()):
	case string_hash(type_name<int64_t>().View()):
	case string_hash(type_name<uint8_t>().View()):
	case string_hash(type_name<uint16_t>().View()):
	case string_hash(type_name<uint32_t>().View()):
	case string_hash(type_name<uint64_t>().View()):
	case string_hash(type_name<float>().View()):
	case string_hash(type_name<double>().View()):
		return true;
	default:
		if constexpr (std::is_same_v<decltype(type_name<double>()), decltype(type_name<long double>())>)
			return false;
		else {
			constexpr std::size_t h = string_hash(type_name<long double>().View());
			return noncv_name_hash == h;
		}
	}
}

constexpr bool Ubpa::type_name_is_fundamental(std::string_view name) noexcept {
	const std::size_t noncv_name_hash = string_hash(type_name_remove_cv(name));
	switch (noncv_name_hash)
	{
	case string_hash(type_name<bool>().View()):
	case string_hash(type_name<int8_t>().View()):
	case string_hash(type_name<int16_t>().View()):
	case string_hash(type_name<int32_t>().View()):
	case string_hash(type_name<int64_t>().View()):
	case string_hash(type_name<uint8_t>().View()):
	case string_hash(type_name<uint16_t>().View()):
	case string_hash(type_name<uint32_t>().View()):
	case string_hash(type_name<uint64_t>().View()):
	case string_hash(type_name<float>().View()):
	case string_hash(type_name<double>().View()):
	case string_hash(type_name<void>().View()):
	case string_hash(type_name<std::nullptr_t>().View()):
		return true;
	default:
		if constexpr (std::is_same_v<decltype(type_name<double>()), decltype(type_name<long double>())>)
			return false;
		else {
			constexpr std::size_t h = string_hash(type_name<long double>().View());
			return noncv_name_hash == h;
		}
	}
}

// properties

constexpr bool Ubpa::type_name_is_const(std::string_view name) noexcept {
	return name.starts_with(std::string_view{"const"}) && name.size() >= 6 && (name[5] == '{' || name[5] == ' ');
}

constexpr bool Ubpa::type_name_is_read_only(std::string_view name) noexcept {
	return type_name_is_const(type_name_remove_reference(name));
}

constexpr bool Ubpa::type_name_is_volatile(std::string_view name) noexcept {
	return name.starts_with(std::string_view{"volatile{"}) || name.starts_with(std::string_view{"const volatile"});
}

constexpr bool Ubpa::type_name_is_cv(std::string_view name) noexcept {
	return name.starts_with(std::string_view{"const volatile"});
}

constexpr bool Ubpa::type_name_is_reference(std::string_view name) noexcept {
	return !name.empty() && name.front() == '&';
}

constexpr bool Ubpa::type_name_is_signed(std::string_view name) noexcept {
	return !type_name_is_unsigned(name);
}

constexpr bool Ubpa::type_name_is_unsigned(std::string_view name) noexcept {
	switch (string_hash(name))
	{
	case string_hash(type_name<uint8_t>().View()):
	case string_hash(type_name<uint16_t>().View()):
	case string_hash(type_name<uint32_t>().View()):
	case string_hash(type_name<uint64_t>().View()):
		return true;
	default:
		return false;
	}
}

constexpr bool Ubpa::type_name_is_bounded_array(std::string_view name) noexcept {
	return name.size() >= 2 && name[0] == '[' && name[1] != ']';
}

constexpr bool Ubpa::type_name_is_unbounded_array(std::string_view name) noexcept {
	return name.size() >= 2 && name[0] == '[' && name[1] == ']';
}

constexpr std::size_t Ubpa::type_name_rank(std::string_view name) noexcept {
	std::size_t rank = 0;
	std::size_t idx = 0;
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

constexpr std::size_t Ubpa::type_name_extent(std::string_view name, std::size_t N) noexcept {
	std::size_t idx = 0;
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

	std::size_t extent = 0;
	while (name[++idx] != ']') {
		assert(idx < name.size());
		extent = 10 * extent + name[idx] - '0';
	}

	return extent;
}

constexpr Ubpa::CVRefMode Ubpa::type_name_cvref_mode(std::string_view name) noexcept {
	if (name.empty())
		return CVRefMode::None;

	if (name[0] == '&') {
		assert(name.size() >= 4);
		if (name[1] == '&') {
			assert(name[2] == '{' && name.back() == '}');
			std::string_view unref_name{ name.data() + 3, name.size() - 4 };
			if (unref_name.starts_with("const")) {
				assert(unref_name.size() >= 6);
				if (unref_name[5] == '{')
					return CVRefMode::ConstRight;
				else if (unref_name[5] == ' ')
					return CVRefMode::CVRight;
				else
					return CVRefMode::Right;
			}
			else if (unref_name.starts_with("volatile{"))
				return CVRefMode::VolatileRight;
			else
				return CVRefMode::Right;
		}
		else {
			assert(name[1] == '{' && name.back() == '}');
			std::string_view unref_name{ name.data() + 2, name.size() - 3 };
			if (unref_name.starts_with("const")) {
				assert(unref_name.size() >= 6);
				if (unref_name[5] == '{')
					return CVRefMode::ConstLeft;
				else if (unref_name[5] == ' ')
					return CVRefMode::CVLeft;
				else
					return CVRefMode::Left;
			}
			else if (unref_name.starts_with("volatile{"))
				return CVRefMode::VolatileLeft;
			else
				return CVRefMode::Left;
		}
	}
	else {
		if (name.starts_with("const")) {
			assert(name.size() >= 6);
			if (name[5] == '{')
				return CVRefMode::Const;
			else if (name[5] == ' ')
				return CVRefMode::CV;
			else
				return CVRefMode::None;
		}
		else if (name.starts_with("volatile{"))
			return CVRefMode::Volatile;
		else
			return CVRefMode::None;
	}
}

// modification (clip)

constexpr std::string_view Ubpa::type_name_remove_cv(std::string_view name) noexcept {
	if (name.starts_with(std::string_view{"const"})) {
		assert(name.size() >= 6);
		if (name[5] == '{') {
			assert(name.back() == '}');
			return { name.data() + 6,name.size() - 7 };
		}
		else if (name[5] == ' ') {
			assert(name.starts_with(std::string_view{"const volatile{"}) && name.back() == '}');
			return { name.data() + 15,name.size() - 16 };
		}
		else
			return name;
	}
	else if (name.starts_with(std::string_view{"volatile{"})) {
		assert(name.back() == '}');
		return { name.data() + 9,name.size() - 10 };
	}
	else
		return name;
}

constexpr std::string_view Ubpa::type_name_remove_const(std::string_view name) noexcept {
	if (!name.starts_with(std::string_view{"const"}))
		return name;

	assert(name.size() >= 6);

	if (name[5] == '{') {
		assert(name.back() == '}');
		return { name.data() + 6,name.size() - 7 };
	}
	else if (name[5] == ' ')
		return { name.data() + 6 };
	else
		return name;
}

constexpr std::string_view Ubpa::type_name_remove_topmost_volatile(std::string_view name) noexcept {
	if (!name.starts_with(std::string_view{"volatile{"}))
		return name;

	assert(name.back() == '}');

	return { name.data() + 9,name.size() - 10 };
}

constexpr std::string_view Ubpa::type_name_remove_lvalue_reference(std::string_view name) noexcept {
	if (name.size() <= 2 || name[0] != '&' || name[1] != '{')
		return name;

	assert(name.size() >= 3 && name.back() == '}');
	return { name.data() + 2, name.size() - 3 };
}

constexpr std::string_view Ubpa::type_name_remove_rvalue_reference(std::string_view name) noexcept {
	if (name.size() <= 2 || name[0] != '&' || name[1] != '&')
		return name;

	assert(name.size() >= 4 && name[2] == '{' && name.back() == '}');
	return { name.data() + 3, name.size() - 4 };
}

constexpr std::string_view Ubpa::type_name_remove_reference(std::string_view name) noexcept {
	if (name.size() <= 2 || name[0] != '&')
		return name;
	
	if (name[1] == '{') {
		assert(name.size() >= 3 && name.back() == '}');
		return { name.data() + 2, name.size() - 3 };
	}
	else {
		assert(name.size() >= 4 && name[1] == '&' && name[2] == '{' && name.back() == '}');
		return { name.data() + 3, name.size() - 4 };
	}
}

constexpr std::string_view Ubpa::type_name_remove_pointer(std::string_view name) noexcept {
	name = type_name_remove_cvref(name);
	if (!name.starts_with(std::string_view{"*"}))
		return name;

	assert(name.size() >= 3 && name[1] == '{' && name.back() == '}');
	return { name.data() + 2, name.size() - 3 };
}

constexpr std::string_view Ubpa::type_name_remove_cvref(std::string_view name) noexcept {
	return type_name_remove_cv(type_name_remove_reference(name));
}

constexpr std::string_view Ubpa::type_name_remove_extent(std::string_view name) noexcept {
	std::size_t idx = 0;

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
		assert(name[idx] == '{' && name.back() == '}');
		return { name.data() + idx + 1, name.size() - idx - 2 };
	}
}

constexpr std::string_view Ubpa::type_name_remove_all_extents(std::string_view name) noexcept {
	if (!type_name_is_array(name))
		return name;

	return type_name_remove_all_extents(type_name_remove_extent(name));
}

constexpr std::size_t Ubpa::type_name_add_const_hash(std::string_view name) noexcept {
	if (type_name_is_reference(name) || type_name_is_const(name))
		return string_hash(name);

	if (type_name_is_volatile(name))
		return string_hash_seed(string_hash("const "), name);
	else
		return string_hash_seed(string_hash_seed(string_hash("const{"), name), "}");
}

constexpr std::size_t Ubpa::type_name_add_volatile_hash(std::string_view name) noexcept {
	if (type_name_is_reference(name) || type_name_is_volatile(name))
		return string_hash(name);

	if (type_name_is_const(name)) {
		name.remove_prefix(5); // {...}
		return string_hash_seed(string_hash("const volatile"), name);
	}
	else
		return string_hash_seed(string_hash_seed(string_hash("volatile{"), name), "}");
}

constexpr std::size_t Ubpa::type_name_add_cv_hash(std::string_view name) noexcept {
	if (type_name_is_reference(name))
		return string_hash(name);

	if (type_name_is_cv(name))
		return string_hash(name);
	
	if (type_name_is_const(name)) {
		name.remove_prefix(5); // {...}
		return string_hash_seed(string_hash("const volatile"), name);
	}
	else if (type_name_is_volatile(name))
		return string_hash_seed(string_hash("const "), name);
	else
		return string_hash_seed(string_hash_seed(string_hash("const volatile{"), name), "}");
}

constexpr std::size_t Ubpa::type_name_add_lvalue_reference_hash(std::string_view name) noexcept {
	if (type_name_is_lvalue_reference(name))
		return string_hash(name);

	if (type_name_is_rvalue_reference(name)) {
		name.remove_prefix(1);
		return string_hash(name);
	}
	else
		return string_hash_seed(string_hash_seed(string_hash("&{"), name), "}");
}

constexpr std::size_t Ubpa::type_name_add_lvalue_reference_weak_hash(std::string_view name) noexcept {
	if (type_name_is_reference(name))
		return string_hash(name);

	return string_hash_seed(string_hash_seed(string_hash("&{"), name), "}");
}

constexpr std::size_t Ubpa::type_name_add_rvalue_reference_hash(std::string_view name) noexcept {
	if(type_name_is_reference(name))
		return string_hash(name);

	return string_hash_seed(string_hash_seed(string_hash("&&{"), name), "}");
}

constexpr std::size_t Ubpa::type_name_add_pointer_hash(std::string_view name) noexcept {
	if (type_name_is_reference(name))
		name = type_name_remove_reference(name);

	return string_hash_seed(string_hash_seed(string_hash("*{"), name), "}");
}

constexpr std::size_t Ubpa::type_name_add_const_lvalue_reference_hash(std::string_view name) noexcept {
	if (type_name_is_reference(name) || type_name_is_const(name))
		return type_name_add_lvalue_reference_hash(name);

	if (type_name_is_volatile(name))
		return string_hash_seed(string_hash_seed(string_hash("&{const "), name), "}");
	else
		return string_hash_seed(string_hash_seed(string_hash("&{const{"), name), "}}");
}

constexpr std::size_t Ubpa::type_name_add_const_rvalue_reference_hash(std::string_view name) noexcept {
	if (type_name_is_reference(name))
		return string_hash(name);

	if (type_name_is_const(name))
		return type_name_add_rvalue_reference_hash(name);

	if (type_name_is_volatile(name))
		return string_hash_seed(string_hash_seed(string_hash("&&{const "), name), "}");
	else
		return string_hash_seed(string_hash_seed(string_hash("&&{const{"), name), "}}");
}

template<typename Alloc>
constexpr std::string_view Ubpa::type_name_add_const(std::string_view name, Alloc alloc) {
	if (type_name_is_reference(name) || type_name_is_const(name))
		return name;

	if (type_name_is_volatile(name)) {
		const std::size_t length = lengthof("const ") + name.size();
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "const ", lengthof("const "));
		std::memcpy(buffer + lengthof("const "), name.data(), name.size());
		return { buffer, length };
	}
	else {
		const std::size_t length = lengthof("const{") + name.size() + lengthof("}");
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "const{", lengthof("const{"));
		std::memcpy(buffer + lengthof("const{"), name.data(), name.size());
		buffer[length - 1] = '}';
		return { buffer, length };
	}
}

template<typename Alloc>
constexpr std::string_view Ubpa::type_name_add_volatile(std::string_view name, Alloc alloc) {
	if (type_name_is_reference(name) || type_name_is_volatile(name))
		return name;

	if (type_name_is_const(name)) {
		name.remove_prefix(5); // {...}
		const std::size_t length = lengthof("const volatile") + name.size();
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "const volatile", lengthof("const volatile"));
		std::memcpy(buffer + lengthof("const volatile"), name.data(), name.size());
		return { buffer, length };
	}
	else {
		const std::size_t length = lengthof("volatile{") + name.size() + lengthof("}");
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "volatile{", lengthof("volatile{"));
		std::memcpy(buffer + lengthof("volatile{"), name.data(), name.size());
		buffer[length - 1] = '}';
		return { buffer, length };
	}
}

template<typename Alloc>
constexpr std::string_view Ubpa::type_name_add_cv(std::string_view name, Alloc alloc) {
	if (type_name_is_reference(name) || type_name_is_cv(name))
		return name;

	if (type_name_is_const(name)) {
		name.remove_prefix(5); // {...}
		const std::size_t length = lengthof("const volatile") + name.size();
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "const volatile", lengthof("const volatile"));
		std::memcpy(buffer + lengthof("const volatile"), name.data(), name.size());
		return { buffer, length };
	}
	else if (type_name_is_volatile(name)) {
		const std::size_t length = lengthof("const ") + name.size();
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "const ", lengthof("const "));
		std::memcpy(buffer + lengthof("const "), name.data(), name.size());
		return { buffer, length };
	}
	else {
		const std::size_t length = lengthof("const volatile{") + name.size() + lengthof("}");
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "const volatile{", lengthof("const volatile{"));
		std::memcpy(buffer + lengthof("const volatile{"), name.data(), name.size());
		buffer[length - 1] = '}';
		return { buffer, length };
	}
}

template<typename Alloc>
constexpr std::string_view Ubpa::type_name_add_lvalue_reference(std::string_view name, Alloc alloc) {
	if (type_name_is_lvalue_reference(name))
		return name;

	if (type_name_is_rvalue_reference(name)) {
		name.remove_prefix(1);
		const std::size_t length = name.size();
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, name.data(), name.size());
		return { buffer, length };
	}
	else {
		const std::size_t length = lengthof("&{") + name.size() + lengthof("}");
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "&{", lengthof("&{"));
		std::memcpy(buffer + lengthof("&{"), name.data(), name.size());
		buffer[length - 1] = '}';
		return { buffer, length };
	}
}

template<typename Alloc>
constexpr std::string_view Ubpa::type_name_add_lvalue_reference_weak(std::string_view name, Alloc alloc) {
	if (type_name_is_reference(name))
		return name;

	const std::size_t length = lengthof("&{") + name.size() + lengthof("}");
	char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
	std::memcpy(buffer, "&{", lengthof("&{"));
	std::memcpy(buffer + lengthof("&{"), name.data(), name.size());
	buffer[length - 1] = '}';
	return { buffer, length };
}

template<typename Alloc>
constexpr std::string_view Ubpa::type_name_add_rvalue_reference(std::string_view name, Alloc alloc) {
	if (type_name_is_reference(name))
		return name;

	const std::size_t length = lengthof("&&{") + name.size() + lengthof("}");
	char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
	std::memcpy(buffer, "&&{", lengthof("&&{"));
	std::memcpy(buffer + lengthof("&&{"), name.data(), name.size());
	buffer[length - 1] = '}';
	return { buffer, length };
}

template<typename Alloc>
constexpr std::string_view Ubpa::type_name_add_pointer(std::string_view name, Alloc alloc) {
	if (type_name_is_reference(name))
		name = type_name_remove_reference(name);

	const std::size_t length = lengthof("*{") + name.size() + lengthof("}");
	char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
	std::memcpy(buffer, "*{", lengthof("*{"));
	std::memcpy(buffer + lengthof("*{"), name.data(), name.size());
	buffer[length - 1] = '}';
	return { buffer, length };
}

template<typename Alloc>
constexpr std::string_view Ubpa::type_name_add_const_lvalue_reference(std::string_view name, Alloc alloc) {
	if (type_name_is_reference(name) || type_name_is_const(name))
		return type_name_add_lvalue_reference(name, alloc);

	if (type_name_is_volatile(name)) {
		const std::size_t length = lengthof("&{const ") + name.size() + lengthof("}");
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "&{const ", lengthof("&{const "));
		std::memcpy(buffer + lengthof("&{const "), name.data(), name.size());
		buffer[length - 1] = '}';
		return { buffer, length };
	}
	else {
		const std::size_t length = lengthof("&{const{") + name.size() + lengthof("}}");
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "&{const{", lengthof("&{const{"));
		std::memcpy(buffer + lengthof("&{const{"), name.data(), name.size());
		buffer[length - 2] = '}';
		buffer[length - 1] = '}';
		return { buffer, length };
	}
}

template<typename Alloc>
constexpr std::string_view Ubpa::type_name_add_const_rvalue_reference(std::string_view name, Alloc alloc) {
	if (type_name_is_reference(name))
		return name;

	if (type_name_is_const(name))
		return type_name_add_rvalue_reference(name, alloc);

	if (type_name_is_volatile(name)) {
		const std::size_t length = lengthof("&&{const ") + name.size() + lengthof("}");
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "&&{const ", lengthof("&&{const "));
		std::memcpy(buffer + lengthof("&&{const "), name.data(), name.size());
		buffer[length - 1] = '}';
		return { buffer, length };
	}
	else {
		const std::size_t length = lengthof("&&{const{") + name.size() + lengthof("}}");
		char* buffer = alloc.allocate(length + 1); buffer[length] = '\0';
		std::memcpy(buffer, "&&{const{", lengthof("&&{const{"));
		std::memcpy(buffer + lengthof("&&{const{"), name.data(), name.size());
		buffer[length - 2] = '}';
		buffer[length - 1] = '}';
		return { buffer, length };
	}
}
