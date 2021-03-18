#pragma once

#include "../Name.hpp"

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
