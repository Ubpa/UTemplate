#include <UTemplate/Type.h>

#include <iostream>

using namespace Ubpa;
using namespace std;

struct A{};

int main() {
	std::cout << Type_of<int>.GetName() << std::endl;
	std::cout << Type_of<float>.GetName() << std::endl;
	std::cout << Type_of<int>.GetID().GetValue() << std::endl;
	std::cout << Type_of<float>.GetID().GetValue() << std::endl;

	static_assert(Type_of<A> == Type{ "A" });
	static_assert(Type_of<const int&>.RemoveCVRef() == Type_of<int>);

	if (Type_of<int> < Type_of<float>)
		cout << "Type_of<int> <  Type_of<float>" << endl;
	if (Type_of<int> <= Type_of<float>)
		cout << "Type_of<int> <= Type_of<float>" << endl;
	if (Type_of<int> > Type_of<float>)
		cout << "Type_of<int> >  Type_of<float>" << endl;
	if (Type_of<int> >= Type_of<float>)
		cout << "Type_of<int> >= Type_of<float>" << endl;
	if (Type_of<int> == Type_of<float>)
		cout << "Type_of<int> == Type_of<float>" << endl;
	if (Type_of<int> != Type_of<float>)
		cout << "Type_of<int> != Type_of<float>" << endl;
}
