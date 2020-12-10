#include <UTemplate/TypeID.h>

#include <iostream>

using namespace Ubpa;
using namespace std;

struct A{};
struct AA {};

int main() {
	cout << TypeID::of<int>.GetValue() << endl;
#ifdef UBPA_NAME_X_INT
	cout << TypeID("int32").GetValue() << endl;
#else
	cout << TypeID("int").GetValue() << endl;
#endif // UBPA_NAME_X_INT

	cout << TypeID::of<float>.GetValue() << endl;
	cout << TypeID("float").GetValue() << endl;
	cout << TypeID::of<A>.GetValue() << endl;
	cout << TypeID("A").GetValue() << endl;
	cout << TypeID::of<AA>.GetValue() << endl;
	cout << TypeID("AA").GetValue() << endl;
}
