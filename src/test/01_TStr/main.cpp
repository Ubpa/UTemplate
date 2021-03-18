#include <iostream>

#include <UTemplate/TStr.hpp>

using namespace std;
using namespace Ubpa;

static constexpr const std::string_view gs_sv = "sv";
static constexpr const char gs_ca[] = "gs_ca";
static constexpr const char* gs_cptr = "gs_cptr";

struct A {
	static constexpr auto str = TSTR("hello");
	static_assert(std::is_same_v<std::remove_cvref_t<decltype(str)>, TStrC_of<'h', 'e', 'l', 'l', 'o'>>);
};

int main() {
	constexpr auto str = TSTR("hello world");
	static_assert(std::is_same_v<std::remove_cvref_t<decltype(str)>, TStrC_of<'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'>>);
	cout << str.View() << endl;
	cout << TSTR(gs_sv).View() << endl;
	cout << TSTR(gs_ca).View() << endl;
	cout << TSTR(gs_cptr).View() << endl;
	return 0;
}
