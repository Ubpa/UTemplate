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
struct ISing : Base {
	using Base::Base;

	void Sing() {
		cout << "Sing";
	}
};

template<typename Base>
struct IJump : Base {
	using Base::Base;

	void Jump() {
		cout << "Jump";
	}
};

template<typename Base>
struct IRap : Base {
	using Base::Base;

	void Rap() {
		cout << "Rap";
	}
};

template<typename Base>
struct IBasketball : Base {
	using Base::Base;

	void Basketball() {
		cout << "Basketball";
	}
};

struct CXK : SII<IBasketball, ISing, IJump, IRap, IPeople>::Ttype<> {
	CXK() : Ttype("CXK") {}
};

int main() {
	CXK cxk;
	cout << "全民制作人们，大家好，我是练习时长两年半的个人练习生" << cxk.name << endl
		<< "喜欢 ";
	cxk.Sing();
	cout << ", ";
	cxk.Jump();
	cout << ", ";
	cxk.Rap();
	cout << ", ";
	cxk.Basketball();
	cout << "!";

	return 0;
}
