#include <iostream>
#include <type_traits>
#include <array>

#include <UTemplate/Basic.h>
#include <UTemplate/Num/Num.h>

using namespace std;
using namespace Ubpa;

template<typename T, T... Vals>
void PrintSequence(sequence<T, Vals...>) {
	cout << Name<T>() << ": ";
	array<T, sizeof...(Vals)> vArr = { Vals... };
	for (auto v : vArr)
		cout << v << ",";
}

int main() {
	PrintSequence(MakeSequence<Size<5>>{});
	return 0;
}
