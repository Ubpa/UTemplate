#include <iostream>
#include <type_traits>

#include <UTemplate/Num.h>
#include <UTemplate/Name.h>

using namespace std;
using namespace Ubpa;


int main() {
	cout << Name<Bool<true>>() << endl;
	cout << Name<Int<1>>() << endl;
	cout << Name<Size<10>>() << endl;

	return 0;
}
