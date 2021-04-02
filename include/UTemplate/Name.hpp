#pragma once

#include "Util.hpp"

#include <string_view>

namespace Ubpa {
	template<auto V>
	constexpr auto constexpr_value_name() noexcept;

	// [rull]
	// - reference : &/&&{...}
	// - cv : const? volatile?{...}
	// - member pointer : {object_type_name}::*{value_type_name}
	// - function : ({arg_1_type_name}, ..., {arg_n_type_name})-{const? volatile? &/&&? noexcept?}->{result_type_name}
	// - int : u?int{8|16|32|64}
	// - bool : bool
	// - float : float{32|64}
	// - template : name<{arg_1_type_name}, ..., {arg_n_type_name}>
	// - enum : enum{...}
	// - union : union{...}
	// - basic : namspace_name::kernal_name
	// [custom] you need to impl get()
	// - custom_type_name
	// - custom_constexpr_value_name
	// - custom_type_namespace_name
	template<typename T>
	constexpr auto type_name() noexcept;

	//
	// constexpr name traits
	//////////////////////////

	constexpr bool constexpr_name_is_null_pointer(std::string_view name) noexcept;
	constexpr bool constexpr_name_is_integral(std::string_view name) noexcept;
	
	//
	// type name traits
	/////////////////////

	// primary

	constexpr bool type_name_is_void(std::string_view name) noexcept;
	constexpr bool type_name_is_null_pointer(std::string_view name) noexcept;
	constexpr bool type_name_is_integral(std::string_view name) noexcept;
	constexpr bool type_name_is_floating_point(std::string_view name) noexcept;
	constexpr bool type_name_is_array(std::string_view name) noexcept;
	constexpr bool type_name_is_enum(std::string_view name) noexcept;
	constexpr bool type_name_is_union(std::string_view name) noexcept;
	constexpr bool type_name_is_function(std::string_view name) noexcept;
	constexpr bool type_name_is_pointer(std::string_view name) noexcept;
	constexpr bool type_name_is_lvalue_reference(std::string_view name) noexcept;
	constexpr bool type_name_is_rvalue_reference(std::string_view name) noexcept;
	constexpr bool type_name_is_member_pointer(std::string_view name) noexcept;

	// composite

	constexpr bool type_name_is_arithmetic(std::string_view name) noexcept;
	constexpr bool type_name_is_fundamental(std::string_view name) noexcept;

	// properties

	constexpr bool type_name_is_const(std::string_view name) noexcept;
	// const{T}, &/&&{T}
	constexpr bool type_name_is_read_only(std::string_view name) noexcept;
	constexpr bool type_name_is_volatile(std::string_view name) noexcept;
	constexpr bool type_name_is_cv(std::string_view name) noexcept;
	constexpr bool type_name_is_reference(std::string_view name) noexcept;
	constexpr bool type_name_is_signed(std::string_view name) noexcept;
	constexpr bool type_name_is_unsigned(std::string_view name) noexcept;
	constexpr bool type_name_is_bounded_array(std::string_view name) noexcept;
	constexpr bool type_name_is_unbounded_array(std::string_view name) noexcept;

	constexpr std::size_t type_name_rank(std::string_view name) noexcept;
	constexpr std::size_t type_name_extent(std::string_view name, std::size_t N = 0) noexcept;

	constexpr CVRefMode type_name_cvref_mode(std::string_view name) noexcept;

	// modification (clip)

	constexpr std::string_view type_name_remove_cv(std::string_view name) noexcept;
	constexpr std::string_view type_name_remove_const(std::string_view name) noexcept;

	constexpr std::string_view type_name_remove_topmost_volatile(std::string_view name) noexcept;

	constexpr std::string_view type_name_remove_lvalue_reference(std::string_view name) noexcept;
	constexpr std::string_view type_name_remove_rvalue_reference(std::string_view name) noexcept;
	constexpr std::string_view type_name_remove_reference(std::string_view name) noexcept;
	constexpr std::string_view type_name_remove_pointer(std::string_view name) noexcept;

	constexpr std::string_view type_name_remove_cvref(std::string_view name) noexcept;

	constexpr std::string_view type_name_remove_extent(std::string_view name) noexcept;

	constexpr std::string_view type_name_remove_all_extents(std::string_view name) noexcept;

	// modification (add, hash)

	constexpr std::size_t type_name_add_const_hash(std::string_view name) noexcept;
	constexpr std::size_t type_name_add_volatile_hash(std::string_view name) noexcept;
	constexpr std::size_t type_name_add_cv_hash(std::string_view name) noexcept;
	constexpr std::size_t type_name_add_lvalue_reference_hash(std::string_view name) noexcept;
	// same with type_name_add_lvalue_reference_hash, but it won't change &&{T}
	constexpr std::size_t type_name_add_lvalue_reference_weak_hash(std::string_view name) noexcept;
	constexpr std::size_t type_name_add_rvalue_reference_hash(std::string_view name) noexcept;
	constexpr std::size_t type_name_add_pointer_hash(std::string_view name) noexcept;
	constexpr std::size_t type_name_add_const_lvalue_reference_hash(std::string_view name) noexcept;
	constexpr std::size_t type_name_add_const_rvalue_reference_hash(std::string_view name) noexcept;

	// modification (add, alloc)

	// if no change, result's data is same with input (no allocate)
	// else allocate (end with '\0')
	// if rst.data() == name.data(), you shouldn't deallocate the result.
	// else you should deallocate it (size : rst.size() + 1)

	template<typename Alloc>
	constexpr std::string_view type_name_add_const(std::string_view name, Alloc alloc);
	template<typename Alloc>
	constexpr std::string_view type_name_add_volatile(std::string_view name, Alloc alloc);
	template<typename Alloc>
	constexpr std::string_view type_name_add_cv(std::string_view name, Alloc alloc);
	template<typename Alloc>
	constexpr std::string_view type_name_add_lvalue_reference(std::string_view name, Alloc alloc);
	template<typename Alloc>
	constexpr std::string_view type_name_add_lvalue_reference_weak(std::string_view name, Alloc alloc);
	template<typename Alloc>
	constexpr std::string_view type_name_add_rvalue_reference(std::string_view name, Alloc alloc);
	template<typename Alloc>
	constexpr std::string_view type_name_add_pointer(std::string_view name, Alloc alloc);
	template<typename Alloc>
	constexpr std::string_view type_name_add_const_lvalue_reference(std::string_view name, Alloc alloc);
	template<typename Alloc>
	constexpr std::string_view type_name_add_const_rvalue_reference(std::string_view name, Alloc alloc);
}

#include "details/Name.inl"
