#include <UTemplate/Name.hpp>

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

template<typename Value, typename T, Value C<T>::* ptr>
struct Ubpa::details::custom_constexpr_value_name<ptr> {
	static constexpr auto get() noexcept {
		if constexpr (member_pointer_equal(ptr, &C<T>::f))
			return TSTR("f");
		else if constexpr (member_pointer_equal(ptr, &C<T>::g))
			return TSTR("g");
		else
			static_assert(always_false<decltype(ptr)>);
	}
};

template<>
struct Ubpa::details::custom_type_namespace_name<C<decltype(&C<float>::f)>::D> {
	static constexpr auto get() noexcept {
		return type_name<C<decltype(&C<float>::f)>>();
	}
};

struct BH {
	void f() {};
};
template<>
struct Ubpa::details::custom_constexpr_value_name<&BH::f> {
	static constexpr auto get() noexcept {
		return TSTR("f");
	}
};
template<auto>
struct B1 {};
template<auto...>
struct B2 {};
template<auto,auto>
struct B3 {};
template<auto, auto, auto>
struct B3_1 {};
template<decltype(&BH::f), decltype(&BH::f)>
struct B4 {};
template<int, decltype(&BH::f), decltype(&BH::f)>
struct B4_1 {};
template<decltype(&BH::f)...>
struct B4_2 {};
template<int, decltype(&BH::f)...>
struct B4_3 {};
template<decltype(&BH::f)>
struct B5 {};
template<decltype(&BH::f), typename T>
struct B6 {};
template<decltype(&BH::f), typename... T>
struct B7 {};
template<typename T, decltype(&BH::f)>
struct B8 {};
template<typename T, decltype(&BH::f), decltype(&BH::f)>
struct B9 {};
template<typename T, decltype(&BH::f), decltype(&BH::f)...>
struct B9_1 {};
template<typename T, decltype(&BH::f)...>
struct B10 {};
template<decltype(&BH::f), typename, typename>
struct B11 {};

enum Enum {};
union Union {};

int main() {
	{ // basic
		std::cout
			<< "//" << std::endl
			<< "// basic" << std::endl
			<< "//////////" << std::endl;

		auto f = []() {};
		std::cout << "|" << type_name<E<&C<int>::g>>().View() << "|" << std::endl;
		std::cout << "|" << type_name<decltype(f)>().View() << "|" << std::endl;
		std::cout << "|" << type_name<A2<int, 2>>().View() << "|" << std::endl;
		std::cout << "|" << type_name<const volatile A<2, int>*const&>().View() << "|" << std::endl;
		std::cout << "|" << type_name<decltype(&f)>().View() << "|" << std::endl;
		std::cout << "|" << type_name<decltype(&C<float>::f)>().View() << "|" << std::endl;
		std::cout << "|" << type_name<C<decltype(&C<float>::f)>::D>().View() << "|" << std::endl;
		std::cout << "|" << type_name<decltype(&C<A<2, int>>::g)>().View() << "|" << std::endl;
		std::cout << "|" << type_name<A<2, int>[][8]>().View() << "|" << std::endl;
		std::cout << "|" << type_name<const AA<A<-2, int>, 2, 4, 5, 126>* const&>().View() << "|" << std::endl;
	}
	{ // to_typename_template_type
		std::cout
			<< "//" << std::endl
			<< "// to_typename_template_type" << std::endl
			<< "//////////////////////////////" << std::endl;

		std::cout << "|" << type_name<B1<&BH::f>>().View() << "|" << std::endl; // 1...
		std::cout << "|" << type_name<B2<&BH::f, &BH::f, &BH::f, &BH::f>>().View() << "|" << std::endl; // 1...
		std::cout << "|" << type_name<B3<&BH::f, &BH::f>>().View() << "|" << std::endl; // 1 1
		std::cout << "|" << type_name<B3_1<&BH::f, &BH::f, &BH::f>>().View() << "|" << std::endl; // 1...
		std::cout << "|" << type_name<B4<&BH::f, &BH::f>>().View() << "|" << std::endl; // 1 1
		std::cout << "|" << type_name<B4_1<1, &BH::f, &BH::f>>().View() << "|" << std::endl; // 1 1 1
		std::cout << "|" << type_name<B4_2<&BH::f, &BH::f, &BH::f, &BH::f>>().View() << "|" << std::endl; // 1...
		std::cout << "|" << type_name<B4_3<1, &BH::f, &BH::f, &BH::f>>().View() << "|" << std::endl; // 1...
		std::cout << "|" << type_name<B5<&BH::f>>().View() << "|" << std::endl; // 1...

		std::cout << "|" << type_name<B6<&BH::f, decltype(&BH::f)>>().View() << "|" << std::endl; // 1 0

		std::cout << "|" << type_name<B7<&BH::f, decltype(&BH::f)>>().View() << "|" << std::endl; // 1 0...
		std::cout << "|" << type_name<B7<&BH::f, decltype(&BH::f), decltype(&BH::f)>>().View() << "|" << std::endl; // 1 0...

		std::cout << "|" << type_name<B8<decltype(&BH::f), &BH::f>>().View() << "|" << std::endl; // 0 1
		std::cout << "|" << type_name<B9<decltype(&BH::f), &BH::f, &BH::f>>().View() << "|" << std::endl; // 0 1 1
		std::cout << "|" << type_name<B9_1<decltype(&BH::f), &BH::f, &BH::f>>().View() << "|" << std::endl; // 0 1 1
		std::cout << "|" << type_name<B10<decltype(&BH::f), &BH::f, &BH::f, &BH::f>>().View() << "|" << std::endl; // 0 1...
		std::cout << "|" << type_name<B11<&BH::f, decltype(&BH::f), decltype(&BH::f)>>().View() << "|" << std::endl; // 1 0 0

		std::cout << "|" << type_name<B8<std::array<decltype(&BH::f), 3>, &BH::f>>().View() << "|" << std::endl; // 1 0 0
	}
	{ // UBPA_NAME_X_INT
		std::cout << "|" << type_name<int8_t>().View() << "|" << std::endl;
		std::cout << "|" << type_name<int16_t>().View() << "|" << std::endl;
		std::cout << "|" << type_name<int32_t>().View() << "|" << std::endl;
		std::cout << "|" << type_name<int64_t>().View() << "|" << std::endl;
		std::cout << "|" << type_name<uint8_t>().View() << "|" << std::endl;
		std::cout << "|" << type_name<uint16_t>().View() << "|" << std::endl;
		std::cout << "|" << type_name<uint32_t>().View() << "|" << std::endl;
		std::cout << "|" << type_name<uint64_t>().View() << "|" << std::endl;
	}
	{ // UBPA_NAME_X_FLOAT
		std::cout << "|" << type_name<float>().View() << "|" << std::endl;
		std::cout << "|" << type_name<double>().View() << "|" << std::endl;
		std::cout << "|" << type_name<long double>().View() << "|" << std::endl;
	}
	{ // TSTR
		static constexpr auto str = TSTR("hello");
		std::cout << "|" << type_name<decltype(str)>().View() << "|" << std::endl;
	}
	{ // enum, union
		std::cout << "|" << type_name<Enum>().View() << "|" << std::endl;
		std::cout << "|" << type_name<Union>().View() << "|" << std::endl;
	}

	return 0;
}
