#include <UTemplate/Name.h>

#include <iostream>

using namespace std;
using namespace Ubpa;

int main() {
	cout << Name<void>() << endl;
	cout << Name<int>() << endl;
	cout << Name<unsigned int>() << endl;
	cout << Name<const int>() << endl;
	cout << Name<volatile int>() << endl;
	cout << Name<int &>() << endl;
	cout << Name<int &&>() << endl;
	cout << Name<int *>() << endl;
	cout << Name<const int * &>() << endl;
	cout << Name<void()>() << endl;
	cout << Name<void(*)(int, int)>() << endl;
	cout << Name<int[2]>() << endl;
	cout << Name<const int[2]>() << endl;
	cout << Name<int[]>() << endl;
	cout << Name<const int[]>() << endl;
	cout << Name<void (* const [2])(int, int)>() << endl;
}
