#include <iostream>
#include <type_traits>

#include <Template/Typelist.h>

using namespace std;
using namespace Ubpa;

int main() {
	using list0 = Typelist<int, float, bool>;
	
	cout << "list0: " << Name<list0>() << endl;
	cout << "Front<list0>: " << Name<Front<list0>>() << endl;
	cout << "PushFront<list0, double>: " << Name<PushFront<list0, double>>() << endl;
	cout << "PopFront<list0>: " << Name<PopFront<list0>>() << endl;
	cout << "PushBack<list0, double>: " << Name<PushBack<list0, double>>() << endl;

	cout << "Clear<list0>: " << Name<Clear<list0>>() << endl;
	cout << "At<list0, 1>: " << Name<At<list0, 1>>() << endl;
	cout << "Accumulate<list0, PushFrontT, Typelist<>>: "
		<< Name<Accumulate<list0, PushFrontT, Typelist<>>>() << endl;

	cout << "Reverse<list0>: " << Name<Reverse<list0>>() << endl;
	cout << "Transform<list0, add_const>: "
		<< Name<Transform<list0, add_const>>() << endl;
	
	cout << "Select<list0, 0, 2>: "
		<< Name<Select<list0, 0, 2>>() << endl;
}
