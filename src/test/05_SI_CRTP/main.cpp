#include <iostream>
#include <type_traits>
#include <array>

#include <UTemplate/SI.h>
#include <UTemplate/Num.h>
#include <UTemplate/TypeList.h>

using namespace std;
using namespace Ubpa;

template<typename Base, typename Impl, typename T, typename Num>
struct IArray : Base, array<T, Num::value> {
	using Base::Base;
private:
	using Base::operator[];
public:
	using array<T, Num::value>::operator[];

	IArray() { }
	template<typename... U>
	IArray(U... data) : array<T, Num::value>{static_cast<T>(data)...} {}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IAdd : SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, Num> {
	using SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, Num>::SIVT_CRTP;

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
struct IIn : SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, Num> {
	using SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, Num>::SIVT_CRTP;

	friend istream& operator>>(istream& is, Impl& x) {
		for (typename Num::type i = 0; i < Num::value; i++) // loop will be optimized in release
			is >> x[i];
		return is;
	}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IOut : SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, Num> {
	using SIVT_CRTP<TemplateList<IArray>, Base, Impl, T, Num>::SIVT_CRTP;

	friend ostream& operator<<(ostream& os, const Impl& x) {
		for (typename Num::type i = 0; i < Num::value - 1; i++) // loop will be optimized in release
			os << x[i] << ", ";
		os << x[Num::value - 1];
		return os;
	}
};

template<typename Base, typename Impl, typename T, typename Num>
struct IInOut : SIVT_CRTP<TemplateList<IOut, IIn>, Base, Impl, T, Num> {
	using SIVT_CRTP<TemplateList<IOut, IIn>, Base, Impl, T, Num>::SIVT_CRTP;
};

template<typename Base, typename Impl, typename T, typename Num>
struct IVal : SIVT_CRTP<TemplateList<IAdd, IInOut>, Base, Impl, T, Num> {
	using SIVT_CRTP<TemplateList<IAdd, IInOut>, Base, Impl, T, Num>::SIVT_CRTP;
};

template<typename T, unsigned N>
struct Vec : SIIT_CRTP<TemplateList<IVal>, Vec<T, N>, T, Size<N>> {
	using SIIT_CRTP<TemplateList<IVal>, Vec<T, N>, T, Size<N>>::SIIT_CRTP;
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
