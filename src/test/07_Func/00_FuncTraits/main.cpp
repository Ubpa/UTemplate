#include <UTemplate/Func.h>

#include <iostream>

using namespace Ubpa;
using namespace std;

struct A {
	void Foo() { cout << "void(A::*)()" << endl; }
	void Foo()const { cout << "void(A::*)() const" << endl; }
	static void Bar() { cout << "void(*)()" << endl; }
	static void Bar(int) { cout << "void(*)(int)" << endl; }
	static void Cat() {}
};

int main() {
	constexpr auto foo = MemFuncOf<void()>::get<A>(&A::Foo);
	constexpr auto cfoo = MemFuncOf<void()const>::get(&A::Foo);
	constexpr auto bar = FuncOf<void()>::get(&A::Bar);
	constexpr auto bari = FuncOf<void(int)>::get(&A::Bar);
	// constexpr auto cat = MemFuncOf<void(int)>::get(&A::Cat);
	// constexpr auto err_foo = MemFuncOf<void(int)>::get(&A::Foo);
	A a;
	(a.*foo)();
	(a.*cfoo)();
	bar();
	bari(1);

	constexpr auto f = DecayLambda([](int) {});
	static_assert(std::is_same_v<decltype(f), void(* const)(int)>);
}
