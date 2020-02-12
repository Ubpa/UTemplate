#include <iostream>
#include <type_traits>

#include <UTemplate/SI.h>

using namespace std;
using namespace Ubpa;

struct Empty1 {};
struct Struct1 { char c; };
struct Derived1 : Empty1 { char c; };
struct Empty2 : Empty1 {};
struct Derived2 : Empty2 { char c; };
struct Empty3 {};
struct Derived3 : Empty2, Empty3 { char c; };
struct Derived4 : Empty2, Empty3 { int c; };
struct Struct2 : Struct1, Empty1 {};

template<typename Base>
struct SIEmpty1 : Base {
	using Empty1 = SIEmpty1;
};
template<typename Base>
struct SIStruct1 : Base {
	using Struct1 = SIStruct1;
	char c;
};
template<typename Base>
struct SIDerived1 : SI<SIEmpty1>::type<Base> { char c; };
template<typename Base>
struct SIEmpty2 : SI<SIEmpty1>::type<Base> {};
template<typename Base>
struct SIDerived2 : SI<SIEmpty2>::type<Base> { char c; };
template<typename Base>
struct SIEmpty3 : Base {};
template<typename Base>
struct SIDerived3 : SI<SIEmpty2, SIEmpty3>::type<Base> { char c; };
template<typename Base>
struct SIDerived4 : SI<SIEmpty2, SIEmpty3>::type<Base> { int c; };
template<typename Base>
struct SIStruct2 : SI<SIStruct1, SIEmpty1>::type<Base> {};

template<typename Base, typename Impl>
struct SITFoo : Base {
	Impl& Foo() {
		return *static_cast<Impl*>(this);
	}
};

template<typename Base, typename Impl>
struct SITGoo : Base {
	void Goo() {}
};

template<typename Base, typename Impl>
struct SITFGoo : SIT<SITFoo, SITGoo>::template type<Base, Impl> { };

template<typename Base, typename Impl>
struct SITHoo : Base {
	void Hoo() {}
};

struct C : SIT<SITFGoo, SITHoo>::type<SINil, C> { };

int main() {
	cout << "1 : " << sizeof(Empty1) << " " << sizeof(SIEmpty1<SINil>) << endl;
	cout << "1 : " << sizeof(Struct1) << " " << sizeof(SIStruct1<SINil>) << endl;
	cout << "1 : " << sizeof(Derived1) << " " << sizeof(SIDerived1<SINil>) << endl;
	cout << "1 : " << sizeof(Empty2) << " " << sizeof(SIEmpty2<SINil>) << endl;
	cout << "1 : " << sizeof(Derived2) << " " << sizeof(SIDerived2<SINil>) << endl;
	cout << "1 : " << sizeof(Derived3) << " " << sizeof(SIDerived3<SINil>) << endl;
	cout << "4 : " << sizeof(Derived4) << " " << sizeof(SIDerived4<SINil>) << endl;
	cout << "1 : " << sizeof(Struct2) << " " << sizeof(SIStruct2<SINil>) << endl;
	Struct2 s2;
	cout << static_cast<Struct2*>(&s2) << endl;
	cout << static_cast<Struct2::Struct1*>(&s2) << endl;
	cout << static_cast<Struct2::Empty1*>(&s2) << endl;
	SIStruct2<SINil> ECBOs2;
	cout << static_cast<SIStruct2<SINil>*>(&ECBOs2) << endl;
	cout << static_cast<SIStruct2<SINil>::Struct1*>(&ECBOs2) << endl;
	cout << static_cast<SIStruct2<SINil>::Empty1*>(&ECBOs2) << endl;

	cout << "[ SIT ]" << endl;
	C c;
	cout << sizeof(C) << endl;

	c.Foo();
	c.Goo();
	c.Hoo();
}
