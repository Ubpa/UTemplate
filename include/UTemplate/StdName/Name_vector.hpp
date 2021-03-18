#pragma once

#include "../Name.hpp"

#include <vector>

template<typename T>
struct Ubpa::details::custom_type_name<std::vector<T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::vector<{"),
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};
