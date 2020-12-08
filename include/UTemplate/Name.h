#pragma once

namespace Ubpa {
	template<auto V>
	constexpr auto constexpr_name() noexcept;
	template<typename T>
	constexpr auto type_name() noexcept;
}

#include "details/Name.inl"
