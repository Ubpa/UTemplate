#include <UTemplate/Type.h>

#include <iostream>

using namespace Ubpa;
using namespace std;

int main() {
	std::cout << Type_of<int>.GetName() << std::endl;
	std::cout << Type_of<float>.GetName() << std::endl;
	std::cout << Type_of<int>.GetTypeID().GetValue() << std::endl;
	std::cout << Type_of<float>.GetTypeID().GetValue() << std::endl;

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
