#include <UTemplate/Func.h>

#include <iostream>

using namespace Ubpa;
using namespace std;

int main() {
	auto expandedFunc = FuncExpand<int&, int&, float&>::run([](int& sum, int n) {
		sum += n;
		});
	int sum = 0;
	int v = 3;
	float tmp;
	expandedFunc(sum, v, tmp);
	cout << sum << endl;
}
