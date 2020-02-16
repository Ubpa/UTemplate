#include <iostream>
#include <type_traits>
#include <array>

#include <UTemplate/SI.h>
#include <UTemplate/Num/Num.h>
#include <UTemplate/List/TypeList.h>

using namespace Ubpa;

template<typename Impl>
struct CRTP_IArray {
	template<typename Base, typename T, typename Num>
	struct Ttype : Base, private std::array<T, Num::value> {
		using Base::Base;

		Ttype() {}
		template<typename... U>
		Ttype(U... data) : std::array<T, Num::value>{static_cast<T>(data)...} {}

		inline T& operator[](size_t _Pos) noexcept {
			return static_cast<std::array<T, Num::value>&>(*this)[_Pos];
		}

		constexpr const T& operator[](size_t _Pos) const noexcept {
			return static_cast<const std::array<T, Num::value>&>(*this)[_Pos];
		}
	};
};

template<typename Impl>
struct CRTP_IAdd {
	template<typename Base, typename T, typename Num>
	struct Ttype : SIVT_CRTP<Base, TypeList<T, Num>, CRTP_IArray<Impl>> {
		using SIVT_CRTP<Base, TypeList<T, Num>, CRTP_IArray<Impl>>::SIVT_CRTP;

		const Impl operator+(const Impl& y) const {
			const Impl& x = *static_cast<const Impl*>(this);
			Impl rst{};
			for (typename Num::type i = 0; i < Num::value; i++) // loop will be optimized in release
				rst[i] = x[i] + y[i];
			return rst;
		}
	};
};

template<typename Impl>
struct CRTP_IIn {
	template<typename Base, typename T, typename Num>
	struct Ttype : SIVT_CRTP<Base, TypeList<T, Num>, CRTP_IArray<Impl>> {
		using SIVT_CRTP<Base, TypeList<T, Num>, CRTP_IArray<Impl>>::SIVT_CRTP;
		friend std::istream& operator>>(std::istream& is, Impl& x) {
			for (typename Num::type i = 0; i < Num::value; i++) // loop will be optimized in release
				is >> x[i];
			return is;
		}
	};
	template<typename T, typename Num>
	struct Ttype<SI_Nil, T, Num> : SIVT_CRTP<SI_Nil, TypeList<T, Num>, CRTP_IArray<Impl>> {
		using SIVT_CRTP<SI_Nil, TypeList<T, Num>, CRTP_IArray<Impl>>::SIVT_CRTP;
	};
};

template<typename Impl>
struct CRTP_IOut {
	template<typename Base, typename T, typename Num>
	struct Ttype : SIVT_CRTP<Base, TypeList<T, Num>, CRTP_IArray<Impl>> {
		using SIVT_CRTP<Base, TypeList<T, Num>, CRTP_IArray<Impl>>::SIVT_CRTP;
		friend std::ostream& operator<<(std::ostream& os, const Impl& x) {
			for (typename Num::type i = 0; i < Num::value - 1; i++) // loop will be optimized in release
				os << x[i] << ", ";
			os << x[Num::value - 1];
			return os;
		}
	};

	template<typename T, typename Num>
	struct Ttype<SI_Nil, T, Num> : SIVT_CRTP<SI_Nil, TypeList<T, Num>, CRTP_IArray<Impl>> {
		using SIVT_CRTP<SI_Nil, TypeList<T, Num>, CRTP_IArray<Impl>>::SIVT_CRTP;
	};
};

template<typename Impl>
struct CRTP_IInOut {
	template<typename Base, typename T, typename Num>
	struct Ttype: SIVT_CRTP<Base, TypeList<T, Num>, CRTP_IIn<Impl>, CRTP_IOut<Impl>> {
		using SIVT_CRTP<Base, TypeList<T, Num>, CRTP_IIn<Impl>, CRTP_IOut<Impl>>::SIVT_CRTP;
	};
};

template<typename Impl>
struct CRTP_IVal {
	template<typename Base, typename T, typename Num>
	struct Ttype : SIVT_CRTP<Base, TypeList<T, Num>, CRTP_IAdd<Impl>, CRTP_IInOut<Impl>> {
		using SIVT_CRTP<Base, TypeList<T, Num>, CRTP_IAdd<Impl>, CRTP_IInOut<Impl>>::SIVT_CRTP;
	};
};

template<typename T, decltype(sizeof(void*)) N>
struct Vec : SIIT<TemplateList<CRTP_IVal<Vec<T,N>>::template Ttype>, T, Size<N>> {
	using SIIT<TemplateList<CRTP_IVal<Vec<T, N>>::template Ttype>, T, Size<N>>::SIIT;
};

using Vecf3 = Vec<float, 3>;
using Vecf100 = Vec<float, 100>;

int main() {
	std::cout << sizeof(Vecf3) << std::endl;
	Vecf3 vs[4];
	Vecf3 v(-1, 3, 4);
	std::cout << v[2];
	for (size_t i = 0; i < 4; i++)
		std::cin >> vs[i];

	std::cout << Vecf3(vs[0][0] + vs[1][0], vs[0][1] + vs[1][1], vs[0][2] + vs[1][2]);
	std::cout << vs[2] + vs[3];
}
