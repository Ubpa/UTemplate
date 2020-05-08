#include <UTemplate/TypeID.h>

#include <iostream>

using namespace Ubpa;
using namespace std;

struct A{};
struct AA {};

int main() {
	static_assert(TypeID<int> != 0);
	cout << TypeID<int> << endl;
	cout << RuntimeTypeID("int") << endl;

	static_assert(TypeID<float> != 0);
	cout << TypeID<float> << endl;
	cout << RuntimeTypeID("float") << endl;

	static_assert(TypeID<A> != 0);
	cout << TypeID<A> << endl;
	cout << RuntimeTypeID("struct A") << endl;

	static_assert(TypeID<AA> != 0);
	cout << TypeID<AA> << endl;
	cout << RuntimeTypeID("struct AA") << endl;
}