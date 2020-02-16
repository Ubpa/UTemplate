#include <iostream>
#include <string>
#include <type_traits>
#include <array>

#include <UTemplate/SI.h>

using namespace std;
using namespace Ubpa;

template<typename Base>
struct IPeople : Base {
	using Base::Base;
	IPeople(const string & name) :name(name) {}
	string name;
};
template<typename Base>
struct ISing : SIV<Base, IPeople> {
	using SIV<Base, IPeople>::Ttype;
	void Sing() { cout << "Sing"; }
};
template<typename Base>
struct IJump : SIV<Base, ISing> {
	using SIV<Base, ISing>::Ttype;
	void Jump() { cout << "Jump"; }
};
template<typename Base>
struct IRap : SIV<Base, IJump> {
	using SIV<Base, IJump>::Ttype;
	void Rap() { cout << "Rap"; }
};
template<typename Base>
struct IBasketball : SIV<Base, IRap> {
	using SIV<Base, IRap>::Ttype;
	void Basketball() { cout << "Basketball"; }
};
struct CXK : SII<IBasketball> { CXK() : Ttype("CXK") {} };
int main() {
	CXK cxk;
	cout << "全民制作人们，大家好，我是练习时长两年半的个人练习生" << cxk.name << endl
		<< "喜欢 ";
	cxk.Sing(); cout << ", "; cxk.Jump(); cout << ", ";
	cxk.Rap(); cout << ", "; cxk.Basketball(); cout << "!";
}
