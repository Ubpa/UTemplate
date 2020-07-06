#include <iostream>
#include <type_traits>

#include <UTemplate/TypeList.h>

using namespace std;
using namespace Ubpa;

int main() {
	using list = TypeList<int, float, bool>;

	static_assert(IsTypeList_v<list>);
	static_assert(!IsTypeList_v<float>);

	static_assert(Length_v<list> == 3);

	static_assert(!IsEmpty_v<list>);
	static_assert(IsEmpty_v<TypeList<>>);

	static_assert(is_same_v<Front_t<list>, int>);
	static_assert(is_same_v<At_t<list, 1>, float>);
	static_assert(is_same_v<Select_t<list, 0, 2>,TypeList<int, bool>>);

	static_assert(Find_v<list, float> == 1);
	static_assert(Find_v<list, char> == Find_fail);

	static_assert(Contain_v<list, int>);
	static_assert(!Contain_v<list, long>);

	static_assert(ContainTs_v<list, int, float>);
	static_assert(!ContainTs_v<list, int, long>);

	static_assert(ContainList_v<list, TypeList<>>);
	static_assert(ContainList_v<list, TypeList<int, float>>);
	static_assert(!ContainList_v<list, TypeList<char, int>>);

	static_assert(CanInstantiate_v<list, TypeList>);
	static_assert(!CanInstantiate_v<list, is_same>);

	static_assert(is_same_v<Instantiate_t<list, TypeList>, list>);
	static_assert(!is_same_v<Instantiate_t<list, TypeList>, TypeList<>>);

	static_assert(ExistInstance_v<TypeList<list>, TypeList>);
	static_assert(!ExistInstance_v<TypeList<list>, is_same>);

	static_assert(is_same_v<SearchInstance_t<TypeList<list>, TypeList>, list>);

	static_assert(is_same_v<PushFront_t<list, double>, TypeList<double, int, float, bool>>);
	static_assert(is_same_v<PushBack_t<list, double>, TypeList<int, float, bool, double>>);
	static_assert(is_same_v<PopFront_t<list>, TypeList<float, bool>>);
	static_assert(is_same_v<Rotate_t<list>, TypeList<float, bool, int>>);
	static_assert(is_same_v<Filter_t<list, is_floating_point>, TypeList<float>>);
	static_assert(is_same_v<Reverse_t<list>, TypeList<bool, float, int>>);
	static_assert(is_same_v<Concat_t<list, TypeList<char, double>>, TypeList<int, float, bool, char, double>>);
	static_assert(is_same_v<Transform_t<list, add_const>, TypeList<const int, const float, const bool>>);
	
	static_assert(IsSet_v<list>);
	static_assert(!IsSet_v<PushFront_t<list, float>>);

	return 0;
}
