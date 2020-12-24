#include <iostream>

#include <UTemplate/TypeID.h>
#include <UTemplate/TypeList.h>

using namespace std;
using namespace Ubpa;

int main() {
	using list = TypeList<int, float, double, TypeList<>>;
	cout << "TypeID<int>        : " << TypeID::of<int>.GetValue() << endl;
	cout << "TypeID<float>      : " << TypeID::of<float>.GetValue() << endl;
	cout << "TypeID<double>     : " << TypeID::of<double>.GetValue() << endl;
	cout << "TypeID<TypeList<>> : " << TypeID::of<TypeList<>>.GetValue() << endl;

	using sorted_list = QuickSort_t<list, TypeID_Less>;
	cout << "@0 : " << TypeID::of<At_t<sorted_list, 0>>.GetValue() << endl;
	cout << "@1 : " << TypeID::of<At_t<sorted_list, 1>>.GetValue() << endl;
	cout << "@2 : " << TypeID::of<At_t<sorted_list, 2>>.GetValue() << endl;
	cout << "@3 : " << TypeID::of<At_t<sorted_list, 3>>.GetValue() << endl;
}
