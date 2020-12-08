#pragma once

// u?int{8|16|32|64}
#define UBPA_NAME_X_INT

namespace Ubpa {
	template<auto V>
	constexpr auto constexpr_name() noexcept;

	// you can custom
	// - member_pointer_name
	// - type_namespace_name
	template<typename T>
	constexpr auto type_name() noexcept;
}

#include "details/Name.inl"
