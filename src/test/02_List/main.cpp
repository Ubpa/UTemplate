#include <iostream>
#include <type_traits>

#include <UTemplate/List/Typelist.h>

using namespace std;
using namespace Ubpa;

int main() {
	using list0 = Typelist<int, float, bool>;
	
	cout << "list0: " << Name<list0>() << endl;
	cout << "Front_t<list0>: " << Name<Front_t<list0>>() << endl;
	cout << "PushFront_t<list0, double>: " << Name<PushFront_t<list0, double>>() << endl;
	//cout << "PushFront<list0, double, char>: " << Name<PushFront<list0, double, char>>() << endl;
	cout << "PopFront_t<list0>: " << Name<PopFront_t<list0>>() << endl;
	cout << "Clear_t<list0>: " << Name<Clear_t<list0>>() << endl;
	cout << "At_t<list0, 1>: " << Name<At_t<list0, 1>>() << endl;
	cout << "Reverse_t<list0>: " << Name<Reverse_t<list0>>() << endl;
	cout << "PushBack_t<list0, double>: " << Name<PushBack_t<list0, double>>() << endl;

	cout << "Transform_t<list0, add_const>: "
		<< Name<Transform_t<list0, add_const>>() << endl;

	cout << "Select_t<list0, 0, 2>: "
		<< Name<Select_t<list0, 0, 2>>() << endl;

	cout << "Contain_v<list0, int>: "
		<< Contain_v<list0, int> << endl;
	cout << "Contain_v<list0, long>: "
		<< Contain_v<list0, long> << endl;

	cout << "ContainList_v<list0, Typelist<>>: "
		<< ContainList_v<list0, Typelist<>> << endl;
	cout << "ContainList_v<list0, Typelist<int, float>>: "
		<< ContainList_v<list0, Typelist<int, float>> << endl;
	cout << "ContainList_v<list0, Typelist<char, int>>: "
		<< ContainList_v<list0, Typelist<char, int>> << endl;

	cout << "Concat_t<list0, Typelist<int, float>>: "
		<< Name<Concat_t<list0, Typelist<int, float>>>() << endl;

	return 0;
}
