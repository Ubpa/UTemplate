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
	constexpr auto foo = MemFuncOf<void()>::run<A>(&A::Foo);
	constexpr auto cfoo = MemFuncOf<void()const>::run(&A::Foo);
	constexpr auto bar = MemFuncOf<void()>::run(&A::Bar);
	constexpr auto bari = MemFuncOf<void(int)>::run(&A::Bar);
	// constexpr auto cat = MemFuncOf<void(int)>::run(&A::Cat);
	// constexpr auto err_foo = MemFuncOf<void(int)>::run(&A::Foo);
	A a;
	(a.*foo)();
	(a.*cfoo)();
	bar();
	bari(1);
}
