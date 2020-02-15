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

template<typename Base>
struct IEmpty1 : Base {};
struct EBCO_Empty1 : SII<IEmpty1> {};
template<typename Base>
struct IStruct1 : Base { char c; };
struct EBCO_Struct1 : SII<IStruct1> {};
template<typename Base>
struct IDerived1 : SIN<Base, IEmpty1> { char c; };
struct EBCO_Derived1 : SII<IDerived1> {};
template<typename Base>
struct IEmpty2 : SIN<Base, IEmpty1> {};
struct EBCO_Empty2 : SII<IEmpty2> {};
template<typename Base>
struct IDerived2 : SIN<Base, IEmpty2> { char c; };
struct EBCO_Derived2 : SII<IDerived2> {};
template<typename Base>
struct IEmpty3 : Base {};
template<typename Base>
struct IDerived3 : SIN<Base, IEmpty2, IEmpty3> { char c; };
struct EBCO_Derived3 : SII<IDerived3> {};
template<typename Base>
struct IDerived4 : SIN<Base, IEmpty2, IEmpty3> { int c; };
struct EBCO_Derived4 : SII<IDerived4> {};
template<typename Base>
struct IStruct2 : SIN<Base, IStruct1, IEmpty1> {};
struct EBCO_Struct2 : SII<IStruct2> {};

int main() {
	cout << "1 : " << sizeof(Empty1) << " " << sizeof(EBCO_Empty1) << endl;
	cout << "1 : " << sizeof(Struct1) << " " << sizeof(EBCO_Struct1) << endl;
	cout << "1 : " << sizeof(Derived1) << " " << sizeof(EBCO_Derived1) << endl;
	cout << "1 : " << sizeof(Empty2) << " " << sizeof(EBCO_Empty2) << endl;
	cout << "1 : " << sizeof(Derived2) << " " << sizeof(EBCO_Derived2) << endl;
	cout << "1 : " << sizeof(Derived3) << " " << sizeof(EBCO_Derived3) << endl;
	cout << "4 : " << sizeof(Derived4) << " " << sizeof(EBCO_Derived4) << endl;
	cout << "1 : " << sizeof(Struct2) << " " << sizeof(EBCO_Struct2) << endl;

	Struct2 s2;
	cout << static_cast<Struct2*>(&s2) << endl;
	cout << static_cast<Struct2::Struct1*>(&s2) << endl;
	cout << static_cast<Struct2::Empty1*>(&s2) << endl;
	IStruct2<SI_Nil> ECBOs2;
	cout << static_cast<IStruct2<SI_Nil>::IStruct1*>(&ECBOs2) << endl;
	cout << static_cast<IStruct2<SI_Nil>::IStruct1*>(&ECBOs2) << endl;
	cout << static_cast<IStruct2<SI_Nil>::IEmpty1*>(&ECBOs2) << endl;
}
