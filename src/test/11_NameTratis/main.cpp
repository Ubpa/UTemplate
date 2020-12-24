#include <UTemplate/Name.h>

using namespace Ubpa;

struct A {
	int data;
	int f(int, int) const noexcept{ return 0; }
};

int main() {

	//
	// constexpr name traits
	//////////////////////////

	static_assert(constexpr_name_is_null_pointer(constexpr_name<nullptr>()));
	static_assert(constexpr_name_is_integral(constexpr_name<32>()));

	//
	// type name traits
	/////////////////////

	// primary

	static_assert(type_name_is_void(type_name<void>()));
	static_assert(type_name_is_null_pointer(type_name<std::nullptr_t>()));
	static_assert(type_name_is_integral(type_name<std::uint64_t>()));
	static_assert(type_name_is_floating_point(type_name<long double>()));
	static_assert(type_name_is_array(type_name<int[][3]>()));
	static_assert(type_name_is_function(type_name<void(int) const&>()));
	static_assert(type_name_is_pointer(type_name<void*>()));
	static_assert(type_name_is_lvalue_reference(type_name<void*&>()));
	static_assert(type_name_is_rvalue_reference(type_name<void*&&>()));
	static_assert(type_name_is_member_pointer(type_name<int A::*>()));

	// properties

	static_assert(type_name_is_const(type_name<const int>()));
	static_assert(type_name_is_volatile(type_name<const volatile int>()));
	static_assert(type_name_is_cv(type_name<const volatile int>()));
	static_assert(type_name_is_signed(type_name<int>()));
	static_assert(type_name_is_signed(type_name<int>()));
	static_assert(type_name_is_bounded_array(type_name<int[3][2]>()));
	static_assert(type_name_is_unbounded_array(type_name<int[][2]>()));
	static_assert(type_name_rank(type_name<int[][3]>()) == 2);
	static_assert(type_name_extent(type_name<int[][3]>(), 1) == 3);

	// modification (clip)

	assert(type_name_remove_cv(type_name<const volatile int>()) == type_name<int>());

	assert(type_name_remove_const(type_name<const volatile int>()) == type_name<volatile int>());

	assert(type_name_remove_topmost_volatile(type_name<volatile int>()) == type_name<int>());

	assert(type_name_remove_reference(type_name<int&>()) == type_name<int>());

	assert(type_name_remove_pointer(type_name<int*>()) == type_name<int>());

	assert(type_name_remove_cvref(type_name<const volatile int&>()) == type_name<int>());

	assert(type_name_remove_extent(type_name<int[][3]>()) == type_name<int[3]>());

	assert(type_name_remove_all_extents(type_name<int[][3]>()) == type_name<int>());

	// modification (add, hash)
	static_assert(type_name_add_const_hash(type_name<int>()) == string_hash(type_name<std::add_const_t<int>>()));
	static_assert(type_name_add_const_hash(type_name<int&>()) == string_hash(type_name<std::add_const_t<int&>>()));
	static_assert(type_name_add_const_hash(type_name<int&&>()) == string_hash(type_name<std::add_const_t<int&&>>()));
	static_assert(type_name_add_const_hash(type_name<const int>()) == string_hash(type_name<std::add_const_t<const int>>()));
	static_assert(type_name_add_const_hash(type_name<const volatile int>()) == string_hash(type_name<std::add_const_t<const volatile int>>()));
	static_assert(type_name_add_const_hash(type_name<volatile int>()) == string_hash(type_name<std::add_const_t<volatile int>>()));

	static_assert(type_name_add_volatile_hash(type_name<int&>()) == string_hash(type_name<std::add_volatile_t<int&>>()));
	static_assert(type_name_add_volatile_hash(type_name<int&&>()) == string_hash(type_name<std::add_volatile_t<int&&>>()));
	static_assert(type_name_add_volatile_hash(type_name<volatile int>()) == string_hash(type_name<std::add_volatile_t<volatile int>>()));
	static_assert(type_name_add_volatile_hash(type_name<const int>()) == string_hash(type_name<std::add_volatile_t<const int>>()));
	static_assert(type_name_add_volatile_hash(type_name<int>()) == string_hash(type_name<std::add_volatile_t<int>>()));

	static_assert(type_name_add_cv_hash(type_name<int&>()) == string_hash(type_name<std::add_cv_t<int&>>()));
	static_assert(type_name_add_cv_hash(type_name<int&&>()) == string_hash(type_name<std::add_cv_t<int&&>>()));
	static_assert(type_name_add_cv_hash(type_name<const volatile int>()) == string_hash(type_name<std::add_cv_t<const volatile int>>()));
	static_assert(type_name_add_cv_hash(type_name<const int>()) == string_hash(type_name<std::add_cv_t<const int>>()));
	static_assert(type_name_add_cv_hash(type_name<volatile int>()) == string_hash(type_name<std::add_cv_t<volatile int>>()));
	static_assert(type_name_add_cv_hash(type_name<int>()) == string_hash(type_name<std::add_cv_t<int>>()));

	static_assert(type_name_add_lvalue_reference_hash(type_name<int&>()) == string_hash(type_name<std::add_lvalue_reference_t<int&>>()));
	static_assert(type_name_add_lvalue_reference_hash(type_name<int&&>()) == string_hash(type_name<std::add_lvalue_reference_t<int&&>>()));
	static_assert(type_name_add_lvalue_reference_hash(type_name<int>()) == string_hash(type_name<std::add_lvalue_reference_t<int>>()));

	static_assert(type_name_add_rvalue_reference_hash(type_name<int&>()) == string_hash(type_name<std::add_rvalue_reference_t<int&>>()));
	static_assert(type_name_add_rvalue_reference_hash(type_name<int&&>()) == string_hash(type_name<std::add_rvalue_reference_t<int&&>>()));
	static_assert(type_name_add_rvalue_reference_hash(type_name<int>()) == string_hash(type_name<std::add_rvalue_reference_t<int>>()));

	static_assert(type_name_add_pointer_hash(type_name<int&>()) == string_hash(type_name<std::add_pointer_t<int&>>()));
	static_assert(type_name_add_pointer_hash(type_name<int&&>()) == string_hash(type_name<std::add_pointer_t<int&&>>()));
	static_assert(type_name_add_pointer_hash(type_name<int>()) == string_hash(type_name<std::add_pointer_t<int>>()));

	// composite

	static_assert(type_name_is_arithmetic(type_name<int>()));
	static_assert(type_name_is_fundamental(type_name<void>()));
}
