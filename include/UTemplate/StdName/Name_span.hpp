#pragma once

#include "../Name.hpp"

#include <span>

template<typename T>
struct Ubpa::details::custom_type_name<std::span<T>> {
	static constexpr auto get() noexcept {
		return concat_seq(
			TSTR("std::span<{"),
			type_name<T>(),
			TStrC_of<'}', '>'>{}
		);
	}
};
