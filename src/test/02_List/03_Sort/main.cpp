#include <iostream>

#include <UTemplate/TypeID/TypeID.h>
#include <UTemplate/List/Typelist.h>
#include <UTemplate/Name.h>

using namespace std;
using namespace Ubpa;

int main() {
	using list = TypeList<int, float, double, TypeList<>>;
	cout << "TypeID<int>        : " << TypeID<int> << endl;
	cout << "TypeID<float>      : " << TypeID<float> << endl;
	cout << "TypeID<double>     : " << TypeID<double> << endl;
	cout << "TypeID<TypeList<>> : " << TypeID<TypeList<>> << endl;

	cout << Name<QuickSort_t<list, TypeID_Less>>() << endl;
}
