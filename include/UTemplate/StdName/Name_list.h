#pragma once

#include "../Name.h"

#include <list>

template<typename T>
struct Ubpa::details::custom_type_name<std::list<T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::list<{"),
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<typename T>
struct Ubpa::details::custom_type_name<std::pmr::list<T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::pmr::list<{"),
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

