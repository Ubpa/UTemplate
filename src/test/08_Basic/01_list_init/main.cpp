#include <UTemplate/Basic.h>

#include <iostream>

using namespace Ubpa;

struct Data {
	unsigned a;
	bool b;
};

int main() {
	static_assert(is_list_initializable_v<Data, Data>);
	static_assert(is_list_initializable_v<Data, unsigned, bool>);
	static_assert(is_list_initializable_v<Data, unsigned>);
	static_assert(is_list_initializable_v<Data, unsigned char, bool>);
	static_assert(is_list_initializable_v<Data, unsigned char>);

	static_assert(!is_list_initializable_v<Data, float>);
	static_assert(!is_list_initializable_v<Data, size_t>);
}
