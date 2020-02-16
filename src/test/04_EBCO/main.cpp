#include <iostream>

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

template<typename Base> struct IEmpty1 : Base {};
template<typename Base> struct IStruct1 : Base { char c; };
template<typename Base> struct IDerived1 : SIN<Base, IEmpty1> { char c; };
template<typename Base> struct IEmpty2 : SIN<Base, IEmpty1> {};
template<typename Base> struct IDerived2 : SIN<Base, IEmpty2> { char c; };
template<typename Base> struct IEmpty3 : Base {};
template<typename Base> struct IDerived3 : SIN<Base, IEmpty2, IEmpty3> { char c; };
template<typename Base> struct IDerived4 : SIN<Base, IEmpty2, IEmpty3> { int c; };
template<typename Base> struct IStruct2 : SIN<Base, IStruct1, IEmpty1> {};

int main() {
	cout << "1 : " << sizeof(Empty1) << " " << sizeof(SII<IEmpty1>) << endl;
	cout << "1 : " << sizeof(Struct1) << " " << sizeof(SII<IStruct1>) << endl;
	cout << "1 : " << sizeof(Derived1) << " " << sizeof(SII<IDerived1>) << endl;
	cout << "1 : " << sizeof(Empty2) << " " << sizeof(SII<IEmpty2>) << endl;
	cout << "1 : " << sizeof(Derived2) << " " << sizeof(SII<IDerived2>) << endl;
	cout << "1 : " << sizeof(Derived3) << " " << sizeof(SII<IDerived3>) << endl;
	cout << "4 : " << sizeof(Derived4) << " " << sizeof(SII<IDerived4>) << endl;
	cout << "1 : " << sizeof(Struct2) << " " << sizeof(SII<IStruct2>) << endl;

	Struct2 s2;
	cout << &s2 << endl;
	cout << static_cast<Struct2::Struct1*>(&s2) << endl;
	cout << static_cast<Struct2::Empty1*>(&s2) << endl;
	IStruct2<SI_Nil> ECBOs2;
	cout << &ECBOs2 << endl;
	cout << static_cast<IStruct2<SI_Nil>::IStruct1*>(&ECBOs2) << endl;
	cout << static_cast<IStruct2<SI_Nil>::IEmpty1*>(&ECBOs2) << endl;
}
