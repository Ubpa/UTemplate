#include <iostream>
#include <type_traits>
#include <array>

#include <UTemplate/SI.h>
#include <UTemplate/Num/Num.h>
#include <UTemplate/List/TypeList.h>

using namespace std;
using namespace Ubpa;

template<typename Base, typename Impl, typename T, typename Num>
struct IArray : Base, array<T, Num::value> {
	using Base::Base;

	IArray() { }
	template<typename... U>
	IArray(U... data) : array<T, Num::value>{static_cast<T>(data)...} {}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IAdd : SIV<IArray>::Ttype<Base, Impl, T, Num> {
	using SIV<IArray>::Ttype<Base, Impl, T, Num>::Ttype;

	const Impl operator+(const Impl& y) const {
		const Impl& x = *static_cast<const Impl*>(this);
		Impl rst{};
		for (typename Num::type i = 0; i < Num::value; i++) // loop will be optimized in release
			rst[i] = x[i] + y[i];
		return rst;
	}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IIn : SIV<IArray>::Ttype<Base, Impl, T, Num> {
	using SIV<IArray>::Ttype<Base, Impl, T, Num>::Ttype;

	friend istream& operator>>(istream& is, Impl& x) {
		for (typename Num::type i = 0; i < Num::value; i++) // loop will be optimized in release
			is >> x[i];
		return is;
	}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IOut : SIV<IArray>::Ttype<Base, Impl, T, Num> {
	using SIV<IArray>::Ttype<Base, Impl, T, Num>::Ttype;

	friend ostream& operator<<(ostream& os, const Impl& x) {
		for (typename Num::type i = 0; i < Num::value - 1; i++) // loop will be optimized in release
			os << x[i] << ", ";
		os << x[Num::value - 1];
		return os;
	}
	float test;
};

template<typename Base, typename Impl, typename T, typename Num>
struct IInOut : SIV<IIn, IOut>::Ttype<Base, Impl, T, Num> {
	using SIV<IIn, IOut>::Ttype<Base, Impl, T, Num>::Ttype;
};

template<typename Base, typename Impl, typename T, typename Num>
struct IVal : SIV<IAdd, IInOut, IArray>::Ttype<Base, Impl, T, Num> {
	using SIV<IAdd, IInOut, IArray>::Ttype<Base, Impl, T, Num>::Ttype;
};

template<typename T, unsigned N>
struct Vec : SII<IVal, IAdd, IInOut, IIn, IOut, IArray>::Ttype<Vec<T, N>, T, Size<N>> {
	using SII<IVal, IAdd, IInOut, IIn, IOut, IArray>::Ttype<Vec<T, N>, T, Size<N>>::Ttype;
};

struct Vecf3 : Vec<float, 3> {
	using Vec<float, 3>::Vec;
};

using Vecf100 = Vec<float, 100>;

int main() {
	cout << sizeof(Vecf3) << endl;
	Vecf3 vs[4];
	Vecf3 u = { 1,2,3 };
	Vecf3 v(-1, 3, 4);

	for (size_t i = 0; i < 4; i++)
		std::cin >> vs[i];

	std::cout << Vecf3({ vs[0][0] + vs[1][0], vs[0][1] + vs[1][1], vs[0][2] + vs[1][2] });
	std::cout << vs[2] + vs[3];

	//Vecf100 v100;
	//v100.fill(2);
	//cout << v100;
}
