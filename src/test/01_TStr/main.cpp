#include <iostream>

#include <UTemplate/TSTR.h>

using namespace std;
using namespace Ubpa;

static constexpr const std::string_view gs_sv = "sv";
static constexpr const char gs_ca[] = "gs_ca";
static constexpr const char* gs_cptr = "gs_cptr";

int main() {
	constexpr auto str0 = TSTR("hello world");
	static_assert(std::is_same_v<decltype(str0), const TStr<char, 'h', 'e', 'l', 'l', 'o', ' ', 'w', 'o', 'r', 'l', 'd'>>);
	cout << str0.name << endl;
	cout << TSTR(gs_sv).name << endl;
	cout << TSTR(gs_ca).name << endl;
	cout << TSTR(gs_cptr).name << endl;
	return 0;
}
