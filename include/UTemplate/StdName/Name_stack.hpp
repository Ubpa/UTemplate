#pragma once

#include "Name_deque.hpp"

#include <stack>

template<typename T>
struct Ubpa::details::custom_type_name<std::stack<T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::stack<{"),
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};
