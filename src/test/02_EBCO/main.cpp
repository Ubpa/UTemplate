#include <iostream>
#include <type_traits>

#include <UTemplate/EBCO.h>

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
struct EBCOEmpty1 : Base {
	using Empty1 = EBCOEmpty1;
};
template<typename Base>
struct EBCOStruct1 : Base {
	using Struct1 = EBCOStruct1;
	char c;
};
template<typename Base>
struct EBCODerived1 : EBCO<EBCOEmpty1>::type<Base> { char c; };
template<typename Base>
struct EBCOEmpty2 : EBCO<EBCOEmpty1>::type<Base> {};
template<typename Base>
struct EBCODerived2 : EBCO<EBCOEmpty2>::type<Base> { char c; };
template<typename Base>
struct EBCOEmpty3 : Base {};
template<typename Base>
struct EBCODerived3 : EBCO<EBCOEmpty2, EBCOEmpty3>::type<Base> { char c; };
template<typename Base>
struct EBCODerived4 : EBCO<EBCOEmpty2, EBCOEmpty3>::type<Base> { int c; };
template<typename Base>
struct EBCOStruct2 : EBCO<EBCOStruct1, EBCOEmpty1>::type<Base> {};

template<typename Base, typename Impl>
struct EBCOIFoo : Base {
	Impl& Foo() {
		return *static_cast<Impl*>(this);
	}
};

template<typename Base, typename Impl>
struct EBCOIGoo : Base {
	void Goo() {}
};

template<typename Base, typename Impl>
struct EBCOIFGoo : EBCOI<EBCOIFoo, EBCOIGoo>::template type<Base, Impl> { };

template<typename Base, typename Impl>
struct EBCOIHoo : Base {
	void Hoo() {}
};

struct C : EBCOI<EBCOIFGoo, EBCOIHoo>::type<EBCONil, C> { };

int main() {
	cout << "1 : " << sizeof(Empty1) << " " << sizeof(EBCOEmpty1<EBCONil>) << endl;
	cout << "1 : " << sizeof(Struct1) << " " << sizeof(EBCOStruct1<EBCONil>) << endl;
	cout << "1 : " << sizeof(Derived1) << " " << sizeof(EBCODerived1<EBCONil>) << endl;
	cout << "1 : " << sizeof(Empty2) << " " << sizeof(EBCOEmpty2<EBCONil>) << endl;
	cout << "1 : " << sizeof(Derived2) << " " << sizeof(EBCODerived2<EBCONil>) << endl;
	cout << "1 : " << sizeof(Derived3) << " " << sizeof(EBCODerived3<EBCONil>) << endl;
	cout << "4 : " << sizeof(Derived4) << " " << sizeof(EBCODerived4<EBCONil>) << endl;
	cout << "1 : " << sizeof(Struct2) << " " << sizeof(EBCOStruct2<EBCONil>) << endl;
	Struct2 s2;
	cout << static_cast<Struct2*>(&s2) << endl;
	cout << static_cast<Struct2::Struct1*>(&s2) << endl;
	cout << static_cast<Struct2::Empty1*>(&s2) << endl;
	EBCOStruct2<EBCONil> ECBOs2;
	cout << static_cast<EBCOStruct2<EBCONil>*>(&ECBOs2) << endl;
	cout << static_cast<EBCOStruct2<EBCONil>::Struct1*>(&ECBOs2) << endl;
	cout << static_cast<EBCOStruct2<EBCONil>::Empty1*>(&ECBOs2) << endl;

	cout << "[ EBCOI ]" << endl;
	C c;
	cout << sizeof(C) << endl;

	c.Foo();
	c.Goo();
	c.Hoo();
}
