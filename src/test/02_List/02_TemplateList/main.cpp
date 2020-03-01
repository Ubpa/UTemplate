#include <type_traits>

#include <UTemplate/TemplateList.h>

using namespace std;
using namespace Ubpa;

template<typename>
struct A {};

template<typename>
struct B {};

template<typename>
struct C {};

template<typename>
struct D {};

template<typename T0, typename T1>
struct T2 {};

int main() {
	using tlist = TemplateList<A, B, C>;

	static_assert(TLength_v<tlist> == 3);

	static_assert(!TIsEmpty_v<tlist>);
	static_assert(TIsEmpty_v<TemplateList<>>);

	static_assert(is_same_v<TPushFront_t<tlist, D>, TemplateList<D, A, B, C>>);

	static_assert(is_same_v<TPushBack_t<tlist, D>, TemplateList<A, B, C, D>>);

	static_assert(is_same_v<TPopFront_t<tlist>, TemplateList<B, C>>);

	static_assert(is_same_v<TReverse_t<tlist>, TemplateList<C, B, A>>);

	static_assert(is_same_v<TConcat_t<tlist, TemplateList<A, D>>, TemplateList<A, B, C, A, D>>);

	static_assert(TExistGenericity_v<tlist, A<int>>);

	static_assert(TExistGenericities_v<tlist, TypeList<A<int>, B<float>>>);

	static_assert(is_same_v<TInstance_t<tlist, TypeList<int>>, TypeList<A<int>, B<int>, C<int>>>);

	static_assert(TCanGeneralizeFromList_v<tlist, TypeList<A<int>, B<float>, C<double>, D<long>>>);

	return 0;
}
