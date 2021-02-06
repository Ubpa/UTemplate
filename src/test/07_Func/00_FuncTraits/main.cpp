#include <UTemplate/Func.h>

#include <iostream>

using namespace Ubpa;
using namespace std;

struct A {
	void Foo(int&) { cout << "void(A::*)(int&)" << endl; }
	void Foo(int&&) { cout << "void(A::*)(int&&)" << endl; }
	void Foo(const int&) { cout << "void(A::*)(const int&)" << endl; }
	void Foo(const int&&) { cout << "void(A::*)(const int&&)" << endl; }
	static void Bar() { cout << "void(*)()" << endl; }
	static void Bar(int) { cout << "void(*)(int)" << endl; }
	static void Cat() {}
};

int main() {
	constexpr auto foo_lref = MemFuncOf<A, void(int&)>::get(&A::Foo);
	constexpr auto foo_rref = MemFuncOf<A, void(int&&)>::get(&A::Foo);
	constexpr auto foo_clref = MemFuncOf<A, void(const int&)>::get(&A::Foo);
	constexpr auto foo_crref = MemFuncOf<A, void(const int&&)>::get(&A::Foo);
	constexpr auto bar = FuncOf<void()>::get(&A::Bar);
	constexpr auto bari = FuncOf<void(int)>::get(&A::Bar);
	// constexpr auto cat = MemFuncOf<void(int)>::get(&A::Cat);
	// constexpr auto err_foo = MemFuncOf<void(int)>::get(&A::Foo);
	int i = 0;
	A a;
	(a.*foo_lref)(i);
	(a.*foo_rref)(std::move(i));
	(a.*foo_clref)(i);
	(a.*foo_crref)(std::move(i));
	bar();
	bari(1);

	constexpr auto f = DecayLambda([](int) {});
	static_assert(std::is_same_v<decltype(f), void(* const)(int)>);
}
