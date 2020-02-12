#include <iostream>
#include <type_traits>
#include <array>

#include <UTemplate/SI.h>
#include <UTemplate/Num.h>
#include <UTemplate/List/Typelist.h>

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
private:
	using VBCs = SIVBCs<typename Base::IArray>;
public:
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
private:
	using VBCs = SIVBCs<typename Base::IArray>;
public:
	using Base::Base;

	friend istream& operator>>(istream& is, Impl& x) {
		for (typename Num::type i = 0; i < Num::value; i++) // loop will be optimized in release
			is >> x[i];
		return is;
	}

private:
	using Dependencies = Typelist<typename Base::IArray>;
};

template<typename Base, typename Impl, typename T, typename Num>
struct IOut : Base {
private:
	using VBCs = SIVBCs<typename Base::IArray>;
public:
	using Base::Base;

	friend ostream& operator<<(ostream& os, const Impl& x) {
		for (typename Num::type i = 0; i < Num::value - 1; i++) // loop will be optimized in release
			os << x[i] << ", ";
		os << x[Num::value - 1];
		return os;
	}

private:
	using Dependencies = Typelist<typename Base::IArray>;
};

template<typename Base, typename Impl, typename T, typename Num>
struct IInOut : SIT<IIn, IOut>::type<Base, Impl, T, Num> {
	using SIT<IIn, IOut>::type<Base, Impl, T, Num>::type;
};

#if 1 // OK
template<typename Base, typename Impl, typename T, typename Num>
struct IVal : SIT<IAdd, IInOut, IArray>::type<Base, Impl, T, Num> {
	using SIT<IAdd, IInOut, IArray>::type<Base, Impl, T, Num>::type;
};
#else // ERROR
template<typename Base, typename Impl, typename T, typename Num>
struct IVal : SIT<IArray, IAdd, IInOut>::type<Base, Impl, T, Num> {
	using SIT<IArray, IAdd, IInOut>::type<Base, Impl, T, Num>::type;
};
#endif

template<typename T, unsigned N>
struct Vec : SIT<IVal>::type<SINil, Vec<T, N>, T, Num<N>> {
	using SIT<IVal>::type<SINil, Vec<T, N>, T, Num<N>>::type;
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
