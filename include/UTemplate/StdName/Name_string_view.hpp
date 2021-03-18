#pragma once

#include "../Name.hpp"

#include <string_view>

template<typename Elem>
struct Ubpa::details::custom_type_name<std::basic_string_view<Elem>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::basic_string_view<{"),
			type_name<Elem>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<>
constexpr auto Ubpa::type_name<std::string_view>() noexcept {
	return TSTR("std::string_view");
}

template<>
constexpr auto Ubpa::type_name<std::wstring_view>() noexcept {
	return TSTR("std::wstring_view");
}

template<>
constexpr auto Ubpa::type_name<std::u8string_view>() noexcept {
	return TSTR("std::u8string_view");
}

template<>
constexpr auto Ubpa::type_name<std::u16string_view>() noexcept {
	return TSTR("std::u16string_view");
}

template<>
constexpr auto Ubpa::type_name<std::u32string_view>() noexcept {
	return TSTR("std::u32string_view");
}
