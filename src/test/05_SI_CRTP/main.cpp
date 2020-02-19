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
struct IAdd : SIV_CRTP<TemplateList<IArray>, Base, Impl, T, Num> {
	using SIV_CRTP<TemplateList<IArray>, Base, Impl, T, Num>::SIV_CRTP;

	const Impl operator+(const Impl& y) const {
		const auto& arr = *SI_Cast<IArray>(this);
		const Impl& x = *static_cast<const Impl*>(this);
		Impl rst{};
		for (typename Num::type i = 0; i < Num::value; i++) // loop will be optimized in release
			rst[i] = x[i] + y[i];
		return rst;
	}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IIn : SIV_CRTP<TemplateList<IArray>, Base, Impl, T, Num> {
	using SIV_CRTP<TemplateList<IArray>, Base, Impl, T, Num>::SIV_CRTP;

	friend istream& operator>>(istream& is, Impl& x) {
		for (typename Num::type i = 0; i < Num::value; i++) // loop will be optimized in release
			is >> x[i];
		return is;
	}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IOut : SIV_CRTP<TemplateList<IArray>, Base, Impl, T, Num> {
	using SIV_CRTP<TemplateList<IArray>, Base, Impl, T, Num>::SIV_CRTP;

	friend ostream& operator<<(ostream& os, const Impl& x) {
		for (typename Num::type i = 0; i < Num::value - 1; i++) // loop will be optimized in release
			os << x[i] << ", ";
		os << x[Num::value - 1];
		return os;
	}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IInOut : SIV_CRTP<TemplateList<IOut, IIn>, Base, Impl, T, Num> {
	using SIV_CRTP<TemplateList<IOut, IIn>, Base, Impl, T, Num>::SIV_CRTP;
};

template<typename Base, typename Impl, typename T, typename Num>
struct IVal : SIV_CRTP<TemplateList<IAdd, IInOut>, Base, Impl, T, Num> {
	using SIV_CRTP<TemplateList<IAdd, IInOut>, Base, Impl, T, Num>::SIV_CRTP;
};

template<typename T, unsigned N>
struct Vec : SII_CRTP<TemplateList<IVal>, Vec<T, N>, T, Size<N>> {
	using SII_CRTP<TemplateList<IVal>, Vec<T, N>, T, Size<N>>::SII_CRTP;
};

using Vecf3 = Vec<float, 3>;
using Vecf100 = Vec<float, 100>;

struct Rst : Vecf3 {};

int main() {
	cout << sizeof(Vecf3) << endl;
	Vecf3 vs[4];

	for (size_t i = 0; i < 4; i++)
		std::cin >> vs[i];

	std::cout << Vecf3(vs[0][0] + vs[1][0], vs[0][1] + vs[1][1], vs[0][2] + vs[1][2]);
	std::cout << vs[2] + vs[3];

	Vecf100 v100;
	v100.fill(2);
	cout << v100 << endl;
}
