#include <UTemplate/Name.h>

#include <iostream>
#include <vector>

using namespace Ubpa;

template<int, typename T>
struct A {};

template<typename T, int...>
struct AA {};

template<typename T, int>
struct A2 {};

template<typename T, int, bool>
struct A3 {};

struct B {
	struct C {};
};

void f(A<2,int>) {
}

template<typename T>
struct C {
	struct D {};
	void f(A<2, int>, A<4, double>) {
	}
	void g(std::vector<int>) const volatile && noexcept {
	}
};

template<auto>
struct E {};

template<typename V1, typename Obj1, typename V2, typename Obj2>
constexpr bool member_pointer_equal(V1 Obj1::* p1, V2 Obj2::* p2) {
	if constexpr (!std::is_same_v<Obj1, Obj2> || !std::is_same_v<V1, V2>)
		return false;
	else
		return p1 == p2;
}

template<typename Value, typename T, Value C<T>::* ptr>
struct Ubpa::details::member_pointer_name<C<T>, Value, ptr> {
	static constexpr auto get() noexcept {
		if constexpr (member_pointer_equal(ptr, &C<T>::f))
			return TSTR("f");
		else if constexpr (member_pointer_equal(ptr, &C<T>::g))
			return TSTR("g");
		else
			static_assert(false);
	}
};

template<>
struct Ubpa::details::type_namespace_name<C<decltype(&C<float>::f)>::D> {
	static constexpr auto get() noexcept {
		return type_name<C<decltype(&C<float>::f)>>();
	}
};

int main() {
	{
		auto f = []() {};
		std::cout << "|" << details::func_signature<E<&C<int>::g>>().name << "|" << std::endl;
		std::cout << "|" << details::raw_type_name<E<&C<int>::g>>().name << "|" << std::endl;
		std::cout << "|" << type_name<E<&C<int>::g>>().name << "|" << std::endl;
		std::cout << "|" << type_name<decltype(f)>().name << "|" << std::endl;
		std::cout << "|" << details::raw_type_name<const volatile A<2, int>* const&>().name << "|" << std::endl;
		std::cout << "|" << details::raw_type_name<const A<2, int>* const&>().name << "|" << std::endl;
		std::cout << "|" << details::raw_type_name<A2<int, 2>>().name << "|" << std::endl;
		std::cout << "|" << type_name<A2<int, 2>>().name << "|" << std::endl;
		std::cout << "|" << type_name<const volatile A<2, int>*const&>().name << "|" << std::endl;
		std::cout << "|" << type_name<decltype(&f)>().name << "|" << std::endl;
		std::cout << "|" << type_name<decltype(&C<float>::f)>().name << "|" << std::endl;
		std::cout << "|" << type_name<C<decltype(&C<float>::f)>::D>().name << "|" << std::endl;
		std::cout << "|" << type_name<decltype(&C<A<2, int>>::g)>().name << "|" << std::endl;
		std::cout << "|" << type_name<A<2, int>[][8]>().name << "|" << std::endl;
		std::cout << "|" << type_name<const AA<A<-2, int>, 2, 4, 5, 126>* const&>().name << "|" << std::endl;
	}
}
