#include <UTemplate/Func.hpp>

#include <iostream>

using namespace Ubpa;
using namespace std;

int main() {
	{ // basic
		auto expandedFunc = FuncExpand<void(int&, int&, float&)>::get([](int& sum, int n) {
			sum += n;
			});
		int sum = 0;
		int v = 3;
		float tmp;
		expandedFunc(sum, v, tmp);
		cout << sum << endl;
	}
	{ // return
		auto expandedFunc = FuncExpand<float(int)>::get([](int n) -> int {
			return n+1;
			});
		cout << expandedFunc(3) << endl;
	}
}
