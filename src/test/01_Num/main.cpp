#include <iostream>
#include <type_traits>

#include <UTemplate/Num/Bool.h>

using namespace std;
using namespace Ubpa;


int main() {
	cout << Name<Bool<true>>() << endl;
	cout << Name<Int<1>>() << endl;
	cout << Name<Size<10>>() << endl;

	cout << "Conjunction<>: " << endl << "\t"
		<< Name<Conjunction<>>() << endl;
	cout << "Conjunction<Int<0>, Long<3>>: " << endl << "\t"
		<< Name<Conjunction<Int<0>, Long<3>>>() << endl;
	cout << "Conjunction<Int<2>, Bool<true>, Size<0>, Long<1>>: " << endl << "\t"
		<< Name<Conjunction<Int<2>, Bool<true>, Size<0>, Long<1>>>() << endl;

	cout << "Disjunction<>: " << endl << "\t"
		<< Name<Disjunction<>>() << endl;
	cout << "Disjunction<Int<0>, Long<3>>: " << endl << "\t"
		<< Name<Disjunction<Int<0>, Long<3>>>() << endl;
	cout << "Disjunction<Int<2>, Bool<true>, Size<0>, Long<1>>: " << endl << "\t"
		<< Name<Disjunction<Int<2>, Bool<true>, Size<0>, Long<1>>>() << endl;

	return 0;
}
