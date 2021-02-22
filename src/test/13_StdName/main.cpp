#include <UTemplate/StdName/Name_memory.h>
#include <UTemplate/StdName/Name_vector.h>

#include <iostream>

using namespace Ubpa;

int main() {
	std::cout << type_name<std::unique_ptr<int>>().Data() << std::endl;
	std::cout << type_name<std::vector<int>>().Data() << std::endl;
}
