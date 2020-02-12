#include <iostream>
#include <type_traits>
#include <array>
#include <UTemplate/EBCO.h>
#include <UTemplate/Num.h>

using namespace std;
using namespace Ubpa;

template<typename Base, typename Impl, typename T, typename Num>
struct IArray : Base, array<T, Num::value> {
	using Base::Base;
	using array<T, Num::value>::array;
	IArray() { }
	template<typename... U>
	IArray(U... data) : array<T, Num::value>{static_cast<T>(data)...} {}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IAdd : Base {
	using Base::Base;

	const Impl operator+(const Impl& y) const {
		const Impl& x = *static_cast<const Impl*>(this);
		Impl rst{};
		for (typename Num::type i = 0; i < Num::value; i++) // loop will be optimized in release
			rst[i] = x[i] + y[i];
		return rst;
	}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IIn : Base {
	using Base::Base;

	friend istream& operator>>(istream& is, Impl& x) {
		for (typename Num::type i = 0; i < Num::value; i++) // loop will be optimized in release
			is >> x[i];
		return is;
	}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IOut : Base {
	using Base::Base;

	friend ostream& operator<<(ostream& os, const Impl& x) {
		for (typename Num::type i = 0; i < Num::value - 1; i++) // loop will be optimized in release
			os << x[i] << ", ";
		os << x[Num::value - 1];
		return os;
	}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IInOut : EBCOI<IIn, IOut>::type<Base, Impl, T, Num> {
	using EBCOI<IIn, IOut>::type<Base, Impl, T, Num>::type;
};

template<typename T, unsigned N>
struct Vec : EBCOI<IArray, IAdd, IInOut>::type<EBCONil, Vec<T,N>, T, Num<N>> {
	using EBCOI<IArray, IAdd, IInOut>::type<EBCONil, Vec<T, N>, T, Num<N>>::type;
};

using Vecf3 = Vec<float, 3>;
using Vecf100 = Vec<float, 100>;

int main() {
	cout << sizeof(Vecf3) << endl;
	Vecf3 v(1, 2, 3);
	Vecf3 u;
	cin >> u;

	v.fill(2);
	v[2] = 0;
	cout << v + u;

	Vecf100 v100;
	v100.fill(2);
	cout << v100;
}
