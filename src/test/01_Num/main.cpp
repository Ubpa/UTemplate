#include <iostream>
#include <type_traits>

#include <UTemplate/Num.h>

using namespace std;
using namespace Ubpa;


int main() {
	Bool<true>{};
	Int<1>{};
	Size<10>{};
	return 0;
}
