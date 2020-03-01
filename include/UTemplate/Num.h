#pragma once

#include "Basic.h"

namespace Ubpa {
	// type, value
	template<typename T, T N> struct Num;

	template<bool b> using Bool = Num<bool, b>;
	using True = Num<bool, true>;
	using False = Num<bool, false>;

	template<char c> using Char = Num<char, c>;
	template<short N> using Short = Num<short, N>;
	template<int N> using Int = Num<int, N>;
	template<long N> using Long = Num<long, N>;
	template<size_t N> using Size = Num<size_t, N>;
}

// ============================================================

namespace Ubpa {
	template<typename T, T N>
	struct Num {
		using type = T;
		static constexpr unsigned value = N;
	};

	template<typename... Ts>
	struct Name;

	template<typename T, T N>
	struct Name<Num<T, N>> {
		friend std::ostream& operator<<(std::ostream& os, Name<Num<T, N>>) {
			os << "Num<" << Name<T>() << ", " << N << ">";
			return os;
		}
	};

	template<bool b>
	struct Name<Num<bool, b>> {
		friend std::ostream& operator<<(std::ostream& os, Name<Num<bool, b>>) {
			os << "Num<" << Name<bool>() << ", " << (b ? "true" : "false") << ">";
			return os;
		}
	};
}
