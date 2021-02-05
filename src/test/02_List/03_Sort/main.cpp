#include <iostream>

#include <UTemplate/Type.h>
#include <UTemplate/TypeList.h>

using namespace std;
using namespace Ubpa;

int main() {
	using list = TypeList<int, float, double, TypeList<>>;
	cout << "TypeID<int>        : " << TypeID_of<int>.GetValue() << endl;
	cout << "TypeID<float>      : " << TypeID_of<float>.GetValue() << endl;
	cout << "TypeID<double>     : " << TypeID_of<double>.GetValue() << endl;
	cout << "TypeID<TypeList<>> : " << TypeID_of<TypeList<>>.GetValue() << endl;

	using sorted_list = QuickSort_t<list, TypeID_Less>;
	cout << "@0 : " << TypeID_of<At_t<sorted_list, 0>>.GetValue() << endl;
	cout << "@1 : " << TypeID_of<At_t<sorted_list, 1>>.GetValue() << endl;
	cout << "@2 : " << TypeID_of<At_t<sorted_list, 2>>.GetValue() << endl;
	cout << "@3 : " << TypeID_of<At_t<sorted_list, 3>>.GetValue() << endl;
}
