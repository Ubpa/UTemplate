#pragma once

#include "../Name.h"

#include <forward_list>

template<typename T>
struct Ubpa::details::custom_type_name<std::forward_list<T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::forward_list<{"),
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<typename T>
struct Ubpa::details::custom_type_name<std::pmr::forward_list<T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::pmr::forward_list<{"),
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};
