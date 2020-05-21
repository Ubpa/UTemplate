#include <UTemplate/Basic.h>

#include <iostream>

using namespace Ubpa;

struct CPrivate {
private:
	CPrivate() = default;
};

struct CProtected {
protected:
	CProtected() = default;
};

struct CPublic {
public:
	CPublic() = default;
};

int main() {
	static_assert(std::is_constructible_v<CPrivate> == false);
	static_assert(std::is_constructible_v<CProtected> == false);
	static_assert(std::is_constructible_v<CPublic> == true);
	static_assert(is_derived_constructible_v<CPrivate> == false);
	static_assert(is_derived_constructible_v<CProtected> == true);
	static_assert(is_derived_constructible_v<CPublic> == true);
}
