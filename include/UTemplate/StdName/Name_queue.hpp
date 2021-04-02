#pragma once

#include "Name_deque.hpp"

#include <deque>

template<typename T>
struct Ubpa::details::custom_type_name<std::deque<T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::deque<{"),
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<typename T>
struct Ubpa::details::custom_type_name<std::priority_queue<T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::priority_queue<{"),
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};

template<typename T, typename Container>
struct Ubpa::details::custom_type_name<std::priority_queue<T, Container>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::priority_queue<{"),
			type_name<T>(),
			TStr_of_a<','>{},
			type_name<Container>(),
			TStrC_of<'}', '>'>{}
		);
	}
};
