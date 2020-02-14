#include <iostream>
#include <type_traits>
#include <string>

#include <UTemplate/List/TemplateList.h>
#include <UTemplate/Name.h>

using namespace std;
using namespace Ubpa;

template<typename = void>
struct A {
	template<typename = void>
	struct Double {
		string name = "AA";
	};
	string name = "A";
};

template<typename = void>
struct B {
	template<typename = void>
	struct Double {
		string name = "BB";
	};
	string name = "B";
};

template<typename = void>
struct C {
	template<typename = void>
	struct Double {
		string name = "CC";
	};
	string name = "C";
};

template<typename = void>
struct D {
	template<typename = void>
	struct Double {
		string name = "DD";
	};
	string name = "D";
};

template<typename ArgList>
struct Name<TemplateList<ArgList>> {
	friend std::ostream& operator<<(std::ostream& os, Name<TemplateList<ArgList>>) {
		return os;
	}
};
template<typename ArgList, template<typename...> class T>
struct Name<TemplateList<ArgList, T>> {
	friend std::ostream& operator<<(std::ostream& os, Name<TemplateList<ArgList, T>>) {
		os << T<void>().name << " | " << Name<ArgList>();
		return os;
	}
};
template<typename ArgList, template<typename...>class... Ts>
struct Name<TemplateList<ArgList, Ts...>> {
	friend std::ostream& operator<<(std::ostream& os, Name<TemplateList<ArgList, Ts...>>) {
		os << TFront<TemplateList<ArgList, Ts...>>::template Ttype<void>().name << ", "
			<< Name<TPopFront_t<TemplateList<ArgList, Ts...>>>();
		return os;
	}
};

template<typename I, typename List, size_t Index>
using ToI = TPushFront<I, TAt<List, Index>::template Ttype>;

template<template<typename...>class T>
struct Double {
	template<typename... Ts>
	using Ttype = typename T<void>::template Double<Ts...>;
};

template<typename NonVoid>
struct Test {
	NonVoid a;
};

int main() {
	using list0 = TemplateList<TypeList<void>, A, B, C>;

	cout << "list0" << endl
		<< "\t" << Name<list0>() << endl;
	cout << "TPushFront_t<list0, D>" << endl
		<< "\t" << Name<TPushFront_t<list0, D>>() << endl;
	cout << "TPopFront_t<list0>" << endl
		<< "\t" << Name<TPopFront_t<list0>>() << endl;
	cout << "TAt<list0, 1>" << endl
		<< "\t" << TAt<list0, 1>::Ttype<>().name << endl;
	cout << "TReverse_t<list0>" << endl
		<< "\t" << Name<TReverse_t<list0>>() << endl;
	cout << "TPushBack_t<list0, D>" << endl
		<< "\t" << Name<TPushBack_t<list0, D>>() << endl;
	cout << "TConcat_t<list0, TemplateList<A,D>>" << endl
		<< "\t" << Name<TConcat_t<list0, TemplateList<TypeList<void>, A,D>>>() << endl;
	cout << "TAccumulateIS_t<list0, ToI, TemplateList<D>, 0, 2>" << endl
		<< "\t" << Name<TAccumulateIS_t<list0, ToI, TemplateList<TypeList<void>, D>, 0, 2>>() << endl;
	cout << "TSelect_t<list0, 0, 2>" << endl
		<< "\t" << Name<TSelect_t<list0, 0, 2>>() << endl;
	cout << "TSelect_t<list0, 0, 2>" << endl
		<< "\t" << Name<TTransform_t<list0, Double>>() << endl;
	cout << "TContain<list0, B>" << endl
		<< "\t" << TContain_v<list0, B> << endl;
	cout << "TContain<list0, D>" << endl
		<< "\t" << TContain_v<list0, D> << endl;
	cout << "TContainList_v<list0, TemplateList<TypeList<void>, A, B>>" << endl
		<< "\t" << TContainList_v<list0, TemplateList<TypeList<void>, A, B>> << endl;
	cout << "TContainList_v<list0, TemplateList<TypeList<void>, D, B>>" << endl
		<< "\t" << TContainList_v<list0, TemplateList<TypeList<void>, D, B>> << endl;
	return 0;
}
